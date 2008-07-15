
/** Source file for StylusCalibration node.
 *
 * @author Stefan Mooslechner
 */   


// This flag generates extra output about the 
// time needed to calibrate, and writes it
// to result.txt.
//#define TEST_AND_EVALUATE_STYLUS_CALIBRATION
#ifndef OT_NO_PROPREGISTRATIONMOUDLE_SUPPORT
#include <OpenTracker/prop/StylusCalibration.h>
#include <OpenTracker/prop/tnt/tnt.h>
#include <OpenTracker/prop/tnt/jama_svd.h>

#include <iomanip> 
#include <fstream>
#include <math.h>

#include <ace/Log_Msg.h>
#include <OpenTracker/tool/OT_ACE_Log.h>

using std::cout;
using std::endl;

#ifdef WIN32
#  include <winbase.h>
#  include <conio.h>
#  define PAGE_UP 0xe049
#  define PAGE_DOWN 0xe051
#  define ESCAPE 0x001b
#else
#  include <unistd.h>
#  define PAGE_UP KEY_PPAGE
#  define PAGE_DOWN KEY_NPAGE
#  define ESCAPE 27
#  if defined(USE_NCURSES) && !defined(RENAMED_NCURSES)
#     include <ncurses.h>
#  else
#     include <curses.h>
#  endif
#endif

#ifdef TEST_AND_EVALUATE_STYLUS_CALIBRATION
#include <time.h>
 char start_time[100];
 char first_point_time[100];
 char second_point_time[100];
 char finish_time[100];
 tm* myTime;
#endif

namespace ot {

   //---------------------------------------------------------------------------
   //
   StylusCalibration::StylusCalibration(int number_of_samples_, std::string filename_, bool show_scatter_, bool save_result_ ) :
     number_of_samples ( number_of_samples_ ), filename( filename_ ), show_scatter( show_scatter_ ), save_result( save_result_ )
   {
      if (number_of_samples == 0)
         number_of_samples = STANDARD_NUMBER_OF_SAMPLES;

      logPrintI("Stylus Calibration initialized with:\n");
      logPrintI("   -> number of Samples: %d\n",number_of_samples);
      logPrintI("   -> file to modifiy: %s\n",filename.c_str());
      logPrintI("   -> show ScatterPlot: %d\n", show_scatter);
      logPrintI("   -> saving results: %d\n", save_result);

      init_point_a = TNT::Matrix<double>(3,1);
      init_point_b = TNT::Matrix<double>(3,1);
      mean_sample = TNT::Matrix<double>(3,1);
      result_svd = TNT::Matrix<double>(3,1);

      minimal_distance = -1.0;
      status = WAIT_4_EVENT;
      print_point2console = false; //just 4 testing and debugging
#ifdef TEST_AND_EVALUATE_STYLUS_CALIBRATION
      time_t myTime0 = time(0);
      myTime = localtime(&myTime0);
      sprintf(start_time, "Start: %d:%2d:%2d\n",
           myTime->tm_hour, myTime->tm_min, myTime->tm_sec);
#endif
   }


   //---------------------------------------------------------------------------
   //
   void StylusCalibration::handleEvent(Event & event) 
   {
      if (print_point2console)
      {
         logPrintI("Point: %.15f %.15f %.15f\n",event.getPosition()[0],event.getPosition()[1],event.getPosition()[2]);
         print_point2console = false;
      }
      if (status == ALL_FINISHED)
         exit(0);
         //return;
      else if (status == WAIT_4_1st_POINT || status == WAIT_4_2nd_POINT)
      {
         checkKeyboard();    
         return;
      }
      else if (status == COLLECT_SAMPLES)
      { 
         checkKeyboard();
         updateSampleVector( event );
         
         if (translation_samples.size() >= number_of_samples)
         {
            logPrintI("Recording finished. Starting calculation.\n");
#ifdef WIN32
            ::Beep(825,200);
#endif
#ifdef TEST_AND_EVALUATE_STYLUS_CALIBRATION
         time_t myTime0 = time(0);
         myTime = localtime(&myTime0);
         sprintf(finish_time, "Finish: %d:%2d:%2d\n",
         myTime->tm_hour, myTime->tm_min, myTime->tm_sec);
#endif

            calcStylus();
            if (filename != "")
            {
               modifyFile();
               logPrintI("ART-Configuration file \"%s\" modified.\n", filename.c_str());
            }
            if (save_result)
            {
               writeResult();
               logPrintI("Result saved to \"result.txt.\"\n");
            }
            if (show_scatter)
            {
               writeScatterPlot();
            }
          
#ifdef WIN32
            if (show_scatter)
               showScatter();
#else
      // so far this works just with windows
      // you can check the scatterplot manually ("./plot/scatter.iv")
#endif

            status = ALL_FINISHED;
         }
         return;
      }
      else if (status == WAIT_4_EVENT)
      {
         logPrintI("\n");
         logPrintI("Hold the Stylus vertical and press 'v'\n");
         logPrintI("or 'page up' to continue\n");
         logPrintI("or 'e' to escape.\n");
         logPrintI("\n");
         status = WAIT_4_1st_POINT;
         return;
      }
      else if (status == RECORD_1st_POINT)
      {
         init_point_a[0][0] = event.getPosition()[0];
         init_point_a[1][0] = event.getPosition()[1];
         init_point_a[2][0] = event.getPosition()[2];
#ifdef TEST_AND_EVALUATE_STYLUS_CALIBRATION
         time_t myTime0 = time(0);
         myTime = localtime(&myTime0);
         sprintf(first_point_time, "1st Point: %d:%2d:%2d\n",
              myTime->tm_hour, myTime->tm_min, myTime->tm_sec);
#endif
#ifdef WIN32
         ::Beep(825,200);
#endif
         logPrintI("\n");
         logPrintI("Now move the Stylus about 45 degree in any direction\n");
         logPrintI("and press 'c' or 'page down' to continue\n");
         logPrintI("or 'e' to escape.\n");
         logPrintI("\n");
         status = WAIT_4_2nd_POINT;
         return;
      }
      else if (status == RECORD_2nd_POINT)
      {
         init_point_b[0][0] = event.getPosition()[0];
         init_point_b[1][0] = event.getPosition()[1];
         init_point_b[2][0] = event.getPosition()[2];
#ifdef TEST_AND_EVALUATE_STYLUS_CALIBRATION
         time_t myTime0 = time(0);
         myTime = localtime(&myTime0);
         sprintf(second_point_time, "2nd Point: %d:%2d:%2d\n",
              myTime->tm_hour, myTime->tm_min, myTime->tm_sec);
#endif
#ifdef WIN32
         ::Beep(825,200);
#endif
         logPrintI("Now move Stylus around.\n");
         status = COLLECT_SAMPLES;
         return;
      }
      else
         assert(false);
   }


   //---------------------------------------------------------------------------
   //
   void StylusCalibration::checkKeyboard()
   {
      int key = 0;
#ifdef WIN32
	  ::Sleep(100);
      while( _kbhit() )
      {
         key = _getch();
         if( key == 0 || key == 0xE0 )
         {
             key = 0xE0 << 8;
             key |= _getch();
         }
#else
        while( (key = getch()) != ERR )
        {
#endif
      }
      switch(key)
      {
	  case 'v':
      case PAGE_UP:
         if (status == WAIT_4_1st_POINT)
            status = RECORD_1st_POINT;
		 break;
	  case 'c':
      case PAGE_DOWN:
         if (status == WAIT_4_2nd_POINT)
            status = RECORD_2nd_POINT;
         break;
      case 'e':
      case ESCAPE:
         logPrintW("Process aborted\n");
         status = ALL_FINISHED;
         break;
      case 'o':
         print_point2console = true;
         break;
 
      default:
         break;
      }
    }


    //---------------------------------------------------------------------------
    //
    void StylusCalibration::updateSampleVector( Event & event )
    {
       bool distance_okay = true;

       TNT::Matrix<double> event_position(3,1);
       event_position[0][0] = event.getPosition()[0];
       event_position[1][0] = event.getPosition()[1];
       event_position[2][0] = event.getPosition()[2];

       for (unsigned int i=0; i<translation_samples.size(); ++i)
       {
          if (!checkDistance(translation_samples[i],event_position))
          {
             distance_okay = false;
             break;
          }
       }
       if (distance_okay)
       {
          TNT::Matrix<double> tmp_translation(3,1);
          tmp_translation[0][0] = event.getPosition()[0];
          tmp_translation[1][0] = event.getPosition()[1];
          tmp_translation[2][0] = event.getPosition()[2];
          translation_samples.push_back(tmp_translation);
          
          TNT::Matrix<double> tmp_orientation(1,4);
          tmp_orientation[0][0] = event.getOrientation()[0];
          tmp_orientation[0][1] = event.getOrientation()[1];
          tmp_orientation[0][2] = event.getOrientation()[2];
          tmp_orientation[0][3] = event.getOrientation()[3];
          orientation_samples.push_back(tmp_orientation);
         
          logPrintI("Samples accepted: %d\n" , translation_samples.size());
       }       
    }


   //---------------------------------------------------------------------------
   //
    void StylusCalibration::calcStylus()
    {
       
       for(unsigned int i=0; i<translation_samples.size(); ++i)
       {
          mean_sample[0][0] += translation_samples[i][0][0];
          mean_sample[1][0] += translation_samples[i][1][0];
          mean_sample[2][0] += translation_samples[i][2][0];
       }
       mean_sample[0][0] /= static_cast<float>(translation_samples.size());
       mean_sample[1][0] /= static_cast<float>(translation_samples.size());
       mean_sample[2][0] /= static_cast<float>(translation_samples.size());

       TNT::Matrix<double> offset_svd(3,1);
       offset_svd = SVDStylusOptimization();

       logPrintI("\n");
       logPrintI("Calculated with SVD:\n");
       logPrintI("Stylus offset: %.10f %.10f %.10f\n", offset_svd[0][0],offset_svd[1][0],offset_svd[2][0]);
       logPrintI("Stylus length: %f\n", getLength(offset_svd));
       logPrintI("\n");

       result_svd = offset_svd;
    }


    //---------------------------------------------------------------------------
    //
    bool StylusCalibration::checkDistance(TNT::Matrix<double> const & vector_a, TNT::Matrix<double> const & vector_b)
    {
       if (minimal_distance == -1.0)
          minimal_distance = calculateMinimumDistance();

       if ( getLength(vector_a - vector_b) > minimal_distance )
          return true;
       return false;
    }


    //---------------------------------------------------------------------------
    // 
    float StylusCalibration::calculateMinimumDistance()
    {
       TNT::Matrix<double> tmp_vector(3,1);
       tmp_vector = init_point_a - init_point_b;
       float result = getLength(tmp_vector);
       result /= (sqrt((float)number_of_samples));
       return (result);
    }

    //---------------------------------------------------------------------------
    //
    float StylusCalibration::getLength(TNT::Matrix<double> const & vector)
    {
       return (static_cast<float>( sqrt( vector[0][0]*vector[0][0] + vector[1][0]*vector[1][0] + vector[2][0]*vector[2][0]) ) );
    }

    //---------------------------------------------------------------------------
    //
    void StylusCalibration::modifyFile()
    {
       int number_of_points;
       std::vector<TNT::Matrix<double>> points;
       std::vector<TNT::Matrix<double>> modified_points;
       std::vector<char> text;

       if ( !readFromFile(number_of_points, points, text) )
          logPrintE("Error while reading from file: %s\n", filename.c_str());

       for (unsigned int i=0; i<points.size(); ++i)
       {
          TNT::Matrix<double> tmp_translation(3,1);
          tmp_translation[0][0] = points[i][0][0] + (result_svd[0][0]* -1);
          tmp_translation[1][0] = points[i][1][0] + (result_svd[1][0]* -1);
          tmp_translation[2][0] = points[i][2][0] + (result_svd[2][0]* -1);
          modified_points.push_back(tmp_translation);
       }

       if ( !writeToFile(number_of_points, points, modified_points, text) )
          logPrintE("Error while writing to file: %s\n", filename.c_str());
    }

    //---------------------------------------------------------------------------
    //
    bool StylusCalibration::readFromFile(int & number_of_points, 
                  std::vector<TNT::Matrix<double>> & points, std::vector<char> & text)
    {
       TNT::Matrix<double> translation(3,1);
       std::fstream * file;

       file = new std::fstream( filename.c_str(), ios::in);
       if( file->good() )
       {
          file->setf( std::ios::skipws );
          file->clear();
          *file >> number_of_points;
          for (int i=0; i<number_of_points; ++i)
          {
             file->clear();
             *file >> translation[0][0] >> translation[1][0] >> translation[2][0];
             points.push_back(translation);
          }
          file->unsetf( std::ios::skipws );
          while ( !file->eof() )
          {
             char tmp_char;
             tmp_char = file->get();
             text.push_back(tmp_char);           
          }
          text.pop_back();
       }
       else  
       {
          if ( file != NULL)
            file->close();
          return false;
       }
       if ( file != NULL)
          file->close();
       return true;
    }


    //---------------------------------------------------------------------------
    //
    bool StylusCalibration::writeToFile(int & number_of_points, std::vector<TNT::Matrix<double>> & points,
                              std::vector<TNT::Matrix<double>> & modified_points, std::vector<char> & text)
    {
       std::fstream * file;
       file = new std::fstream( filename.c_str(), std::ios_base::out | std::ios_base::trunc );
       if( file->good() )
       {
          char str[100];
          sprintf(str, "%d\n", number_of_points);
          *file << str << std::flush;

          for(unsigned int i=0; i<points.size(); ++i)
          {
             sprintf(str, "%.10f %.10f %.10f\n",
                  modified_points[i][0][0], 
                  modified_points[i][1][0], 
                  modified_points[i][2][0]);
             *file << str << std::flush;
          }

          *file << std::endl << "# modified by StylusCalibration.cxx" << std::endl << std::flush;
          *file << "# original values:" << std::endl << std::flush;
          for(unsigned int i=0; i<points.size(); ++i)
          {
             sprintf(str, "# %.10f %.10f %.10f\n",
                  points[i][0][0], 
                  points[i][1][0], 
                  points[i][2][0]);
             *file << str << std::flush;
          }

          for (unsigned int i=0; i<text.size(); ++i)
             *file << text[i] << std::flush;
       }
       else
       {
          if ( file != NULL)
            file->close();
          return false;
       }

       if ( file != NULL)
          file->close();
       return true;
    }
    

   //---------------------------------------------------------------------------
   //
   void StylusCalibration::writeScatterPlot()
   {
    std::string scatter_filename = "plot/scatter.iv";
    std::ofstream * output;
    output = new std::ofstream( scatter_filename.c_str(), std::ios_base::out | std::ios_base::trunc );

    float cube_size = minimal_distance * 0.5f;

    if( output->good() )
    {
       char str[420];
       sprintf(str, "#Inventor V2.1 ascii\n\n"
                     "SoSeparator\n"
                     "{\n"
                     "  SoMaterial { diffuseColor 0.965 0.3 0.22 }\n");
       *output << str << std::flush;

       for (unsigned int i=0; i<translation_samples.size();++i)  //print Samples
       {
          sprintf(str, "  SoSeparator\n"
                     "  {\n"
                     "    SoTransform { translation %.15f %.15f %.15f\n }\n"
                     "    SoCube { width %.5f height %.5f depth %.5f }\n"
                     "  }\n",
                  translation_samples[i][0][0], 
                  translation_samples[i][1][0],
                  translation_samples[i][2][0],
                  cube_size, cube_size, cube_size);
          *output << str << std::flush;
       }
       sprintf(str, "}\n");
       *output << str << std::flush;
    }
    else
    {
       logPrintE("Error occured while writing the scatterplot\n");
    }
    if ( output != NULL)
       output->close();
   }


   //---------------------------------------------------------------------------
   //
   void StylusCalibration::showScatter()
   {
      STARTUPINFO sInfo;
      ZeroMemory(&sInfo,sizeof(sInfo));

      PROCESS_INFORMATION pInfo;
      ZeroMemory(&pInfo,sizeof(pInfo));

      sInfo.cb=sizeof(sInfo);

      if(!::CreateProcess(TEXT(".\\plot\\showplot.bat"),NULL,NULL,NULL,true,CREATE_NEW_CONSOLE,NULL,NULL,&sInfo,&pInfo))
         logPrintE("StylusCalibration::showScatter() - CreateProcess failed\n");
   }


   //---------------------------------------------------------------------------
   //
   void StylusCalibration::writeResult()
   {
    std::string result_filename = "result.txt";
    std::ofstream * output;
    output = new std::ofstream( result_filename.c_str(), std::ios_base::out | std::ios_base::trunc );

    if( output->good())
     {
        char str[200];
#ifdef TEST_AND_EVALUATE_STYLUS_CALIBRATION
        sprintf(str, "%s", start_time);
        *output << str << std::flush;
        sprintf(str, "%s", first_point_time);
        *output << str << std::flush;
        sprintf(str, "%s", second_point_time);
        *output << str << std::flush;
        sprintf(str, "%s", finish_time);
        *output << str << std::flush;
#endif
        sprintf(str, "Stylus offset:\n%.15f %.15f %.15f\n", 
                  result_svd[0][0],
                  result_svd[1][0],
                  result_svd[2][0]);
        *output << str << std::flush;
        sprintf(str, "Stylus length: %.5f\n", getLength(result_svd));
        *output << str << std::flush;

        if (filename != "")
        {
        sprintf(str, "%s has been modified\n", filename.c_str());
        *output << str << std::flush;
        }
        else
        {
           sprintf(str, "No file has been modified\n");
           *output << str << std::flush;
        }
        sprintf(str, "\nMinimal distance between samples: %.10f\n", minimal_distance);
        *output << str << std::flush;
        sprintf(str, "Calculation based on the following %d points:\n", number_of_samples);
        *output << str << std::flush;
        sprintf(str, "(translation[0],translation[1],translation[2],orientation[0],orientation[1],orientation[2],orientation[3])\n");
        *output << str << std::flush;
        for(unsigned int i=0; i<translation_samples.size();++i)
        {          
          sprintf(str, "%.15f %.15f %.15f %.15f %.15f %.15f %.15f\n",
                  translation_samples[i][0][0],
                  translation_samples[i][1][0],
                  translation_samples[i][2][0],
                  orientation_samples[i][0][0],
                  orientation_samples[i][0][1],
                  orientation_samples[i][0][2],
                  orientation_samples[i][0][3]);
          *output << str << std::flush;
        }
     }
    else
    {
       logPrintE("Error occured while writing %s\n", filename.c_str());
    }
    if (output != NULL)
       output->close();
   }

   //###########################################################################
   // The following methods based on a furter work of Rainer Splechtna
   // (see StylusCalibration.h for further information)
   //###########################################################################
   //
   //---------------------------------------------------------------------------
   //
    TNT::Matrix<double> StylusCalibration::SVDStylusOptimization()
    {
       float cepsilon = 0.0000001f;
       int n = translation_samples.size();

       TNT::Matrix<double> accumTrans(3,1);
       TNT::Array2D<double> accumRot(3,3,0.0);
       TNT::Matrix<double> rm(4,4);

       for(int i=0; i<n; ++i)
       {
          accumTrans[0][0] += translation_samples[i][0][0];
          accumTrans[1][0] += translation_samples[i][1][0];
          accumTrans[2][0] += translation_samples[i][2][0];

          rm = calculateMatrix(orientation_samples[i]);
          accumRot = matadd(accumRot, generateMatrix(rm));
       }

       TNT::Array2D<double> b(3*n,1,0.0);
       TNT::Array2D<double> A(3*n,3,0.0);
       TNT::Array2D<double> Atmp(3,3,0.0);
       TNT::Array2D<double> x(3,1,0.0);    
       TNT::Matrix<double> bv(3,1);

       for(int i=0; i<n; ++i)
       {
          bv[0][0] = accumTrans[0][0] - (float)n*translation_samples[i][0][0];
          bv[1][0] = accumTrans[1][0] - (float)n*translation_samples[i][1][0];
          bv[2][0] = accumTrans[2][0] - (float)n*translation_samples[i][2][0];

          for(int j=0; j<3; ++j)
             b[i*3+j][0] = bv[j][0];

          rm = calculateMatrix(orientation_samples[i]);
          Atmp = matadd(multscalar(generateMatrix(rm), double(n)), multscalar(accumRot, -1.0));

          for(int j=0; j<3; ++j)
             for (int k=0; k<3; ++k)
                A[i*3+j][k] = Atmp[j][k];
       }

       JAMA::SVD<double> svd(A);
       TNT::Array2D<double> U,V,S;
       svd.getU(U);
       svd.getV(V);
       svd.getS(S);

       for(int i=0; i<3; ++i)
          S[i][i] = S[i][i] > cepsilon ? 1.0/S[i][i] : 0.0;

       x = matmult( V, matmult( S, matmult( U.transpose() , b ) ) );

       TNT::Matrix<double> out(3,1);
       for(int i=0; i<3; ++i)
         out[i][0] = (float)x [i][0];

       return out;
    }


   //---------------------------------------------------------------------------
   //
    TNT::Array2D<double> StylusCalibration::generateMatrix(TNT::Matrix<double> in)
   {
    TNT::Array2D<double> out(3,3);
    for(int m=0; m<3; ++m)
       for(int n=0; n<3; ++n)
          out[m][n] = in[n][m];
    return out;
   }

   //---------------------------------------------------------------------------
   //
    TNT::Matrix<double> StylusCalibration::calculateMatrix(TNT::Matrix<double> const & orientation)
   {
      TNT::Matrix<double> out(4,4);

      double sqx = orientation[0][0] * orientation[0][0];
      double sqy = orientation[0][1] * orientation[0][1];
      double sqz = orientation[0][2] * orientation[0][2];
      double sqw = orientation[0][3] * orientation[0][3];
      double invs = 1 / (sqw + sqx + sqy + sqz);

      out[0][0] = ( sqx - sqy - sqz + sqw)*invs ; 
      out[1][1] = (-sqx + sqy - sqz + sqw)*invs ;
      out[2][2] = (-sqx - sqy + sqz + sqw)*invs ;

      double tmp1 = orientation[0][0] * orientation[0][1];
      double tmp2 = orientation[0][3] * orientation[0][2];
      out[1][0] = 2.0 * (tmp1 - tmp2)*invs ;
      out[0][1] = 2.0 * (tmp1 + tmp2)*invs ;

      tmp1 = orientation[0][0] * orientation[0][2];
      tmp2 = orientation[0][1] * orientation[0][3];
      out[2][0] = 2.0 * (tmp1 + tmp2)*invs ;
      out[0][2] = 2.0 * (tmp1 - tmp2)*invs ;

      tmp1 = orientation[0][1] * orientation[0][2];
      tmp2 = orientation[0][0] * orientation[0][3];
      out[2][1] = 2.0 * (tmp1 - tmp2)*invs ;
      out[1][2] = 2.0 * (tmp1 + tmp2)*invs ;

      out[3][3] = 1.0;
      return out;
   }
   //
   //
   //###########################################################################

} // namespace ot
#endif
