
#include <OpenTracker/prop/PropRegistration.h>
#include <OpenTracker/prop/rigidTransform/RigidTransform.h>

#include <ace/Log_Msg.h>
#include <fstream>


#ifdef WIN32
#  include <winbase.h>
#  include <conio.h>
#  define PAGE_UP 0xe049
#  define PAGE_DOWN 0xe051
#  define ESCAPE 0x001b
#  define BACKSPACE 0x0008
#else
#  include <unistd.h>
#  define PAGE_UP KEY_PPAGE
#  define PAGE_DOWN KEY_NPAGE
#  define ESCAPE 27
#  define BACKSPACE KEY_BACKSPACE
#  if defined(USE_NCURSES) && !defined(RENAMED_NCURSES)
#     include <ncurses.h>
#  else
#     include <curses.h>
#  endif
#endif

namespace ot {

   //---------------------------------------------------------------------------
   //
   PropRegistration::PropRegistration(bool is_stylus_, PropRegistration * antagonist_, 
            std::string prop_file_ ,std::string d_track_file_ ):
            is_stylus(is_stylus_), antagonist(antagonist_), d_track_file(d_track_file_), prop_file(prop_file_)
   {
      if (antagonist == 0)
         connected = false;
      else
         connected = true;

      change_null_point = false;
	  //toggle_keyboard = false;

	  logPrintI("Prop Registration initialized:\n");
      if (is_stylus)
      {
         logPrintI("   -> Node is stylus\n");
         logPrintI("   -> Prop file: %s\n",prop_file.c_str());
         logPrintI("   -> DTrack file to modifiy: %s\n",d_track_file.c_str());
      }
      else
      {
         logPrintI("   -> Node is prop\n");
      }

      if (antagonist == 0)
         logPrintI("   -> no antagonist specified\n"); 
      else
         logPrintI("   -> antagonist specified\n"); 

      init();
   }

//-----------------------------------------------------------------------
//
   void PropRegistration::allFinished()
   {
      logPrintI("Calculation finished. Program stopped.\n");
      exit(0);
   }

//-----------------------------------------------------------------------
//
   bool PropRegistration::loadPropFile()
   {
      std::fstream * file;
      file = new std::fstream( prop_file.c_str(), ios::in);

      if( file->good() )
      {
         file->setf( std::ios::skipws );
         file->clear();
         *file >> number_of_prop_points;

         if (number_of_prop_points < 0)
         {
            number_of_prop_points *= -1;
            change_null_point = true;
         }

         virtual_prop_point_list = TNT::Matrix<double>(3,number_of_prop_points);

         for (int i=0; i<number_of_prop_points; ++i)
         {
            file->clear();
            *file >> virtual_prop_point_list[0][i] >> virtual_prop_point_list[1][i] >> virtual_prop_point_list[2][i];
         }

         if (change_null_point)
         {
            null_point = TNT::Matrix<double>(3,1);
            file->clear();
            *file >> null_point[0][0] >> null_point[1][0] >> null_point[2][0];
         }
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

//-----------------------------------------------------------------------
//
   void PropRegistration::init()
   {
      if (is_stylus) // node is stylus node
      {
         if ( !loadPropFile() )
         {
            logPrintE("Error while reading prop file.\n");
            exit (-1);
         }       
         logPrintI("Points to select: %d\n", number_of_prop_points);

         if ( change_null_point &&  d_track_file == "")
         {
            logPrintW("You selected a NullPoint at the virtual prop, but you\n");
            logPrintW("did not provide a D-Track file to modify.\n");
            logPrintW("The NullPoint will be left in the tracking target.\n");
            change_null_point = false;
         }
         if ( !change_null_point &&  !(d_track_file == "") )
         {
            logPrintW("You provide a D-Track file to modifiy, but there is no\n");
            logPrintW("NullPoint selected at the virtual-prop-file.\n");
            logPrintW("The D-Track file will be ignored.\n");
            change_null_point = false;
         }
      }
      else  // node is prop node
      {
         prop_event_pos = TNT::Matrix<double>(3,1);
         prop_event_orient = TNT::Matrix<double>(1,4);
         prop_event_time = 0.0;
      }
   }

   //---------------------------------------------------------------------------
   //
   bool PropRegistration::checkIfDTrackFileExists()
   {
      if ( d_track_file == "")
         return false;

      std::fstream * file;
      bool return_value = false;

      file = new std::fstream( d_track_file.c_str(), ios::in);
      if( file->good() )
      {
         logPrintE("File is good() \n");
         return_value = true;
      }
      else
         logPrintE("File not good() \n");

      if ( file != NULL)
      {
         file->close();
         logPrintE("cosing file\n");
      }
      return return_value;
   }
   //---------------------------------------------------------------------------
   //
   void PropRegistration::handleEvent(Event & event) 
   {
      if (antagonist != 0 && !antagonist->connected)  // no connection from the first created node 
      {                                               // to the second created node
         antagonist->connectToMe(this);   // build up this connection
      }
      
      if (!is_stylus)  // node is prop node -> store event data
      {
         prop_event_time = event.time;
         prop_event_pos[0][0] = event.getPosition()[0];
         prop_event_pos[1][0] = event.getPosition()[1];
         prop_event_pos[2][0] = event.getPosition()[2];
         prop_event_orient[0][0] = event.getOrientation()[0];
         prop_event_orient[0][1] = event.getOrientation()[1];
         prop_event_orient[0][2] = event.getOrientation()[2];
         prop_event_orient[0][3] = event.getOrientation()[3];
      }
      else  // node is stylus node -> wait for keyboard input
      {
         checkKeyboard(event);
      }
   }

   //---------------------------------------------------------------------------
   //
   void PropRegistration::checkKeyboard(Event & event)
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
      case PAGE_UP:
      case PAGE_DOWN:
		 storePoint(event);
	  	 break;
      case 'c':
         calculateResult();
         break;
      case ESCAPE:
         logPrintW("Process aborted\n");
         exit(0);
         break;
      case BACKSPACE:
         removeLastPoint();
         break;
      default:
         break;
      }
    }

   //---------------------------------------------------------------------------
   //
   void PropRegistration::calculateResult()      
   {
      if (real_prop_point_list.num_cols() != number_of_prop_points)
      {
         logPrintW("The number of stored points differs to the number of needed points.\n");
         logPrintW("Unable to start the calculation.\n");
         return;
      }

      RigidTransform rigid_transform(real_prop_point_list,virtual_prop_point_list);

      rotation = rigid_transform.getR();
      translation = rigid_transform.getT();

      if (change_null_point) // modify DTrack file and we need no translation in the .iv file
      {
         modifyDTrackFile();
         logPrintI("DTrack file was modified. The following translation was added: \n");
         logPrintI("%f %f %f \n", ((translation[0][0]*-1) + null_point[0][0])
								, ((translation[1][0]*-1) + null_point[1][0])
								, ((translation[2][0]*-1) + null_point[2][0]));

         logPrintI("Please add the following lines to the top of the prop.iv file: \n");
		 std::cout << std::endl << "Transform {" << std::endl;
         std::cout << "  translation " << (null_point[0][0]) << " " << (null_point[1][0]) << " " << (null_point[2][0]) << std::endl;
         std::cout << "}" << std::endl << std::endl;
      }
      else
      {
         logPrintI("Please add the following lines to the top of the prop.iv file: \n");
         std::cout << "Transform {" << std::endl;
         std::cout << "  translation " << translation[0][0] << " " << translation[1][0] << " " << translation[2][0] << std::endl;
         std::cout << "}" << std::endl << std::endl;
      } 

      std::cout << "MatrixTransform {" << std::endl;
      std::cout << "matrix " << rotation[0][0] << " " << rotation[0][1] << " " << rotation[0][2] << " 0" << std::endl;
      std::cout << rotation[1][0] << " " << rotation[1][1] << " " << rotation[1][2] << " 0" << std::endl;
      std::cout << rotation[2][0] << " " << rotation[2][1] << " " << rotation[2][2] << " 0" << std::endl;
      std::cout << "0 0 0 1" << std::endl;
      std::cout << "}" << std::endl << std::endl;
      

      calculateMeanError();
      allFinished();
   }

   //---------------------------------------------------------------------------
   //
   void PropRegistration::storePoint(Event & event)
   {    
      if (antagonist ==  0)
      {
         logPrintE("PropRegistration::storePoint() - No antagonist -> No competition\n");
         return;
      }

      if (real_prop_point_list.num_cols() == number_of_prop_points) // needed number of points already stored
      {
         logPrintW("You already stored all needed points.\n");
         logPrintI("Use 'Backslash' to delete points or 'c' to\n");
         logPrintI("calculate the result.\n");
         return;
      }         

      double time_diff = event.time - antagonist->prop_event_time;
       if (time_diff > 250.0)  //if the stored point is 'older' than a quater second
      {
         logPrintW("Time difference between the two events was to long.\n");
         logPrintW("Maybe the prop is not visible?\n");
         logPrintW("Point not stored!\n");
         return;
      }

      TNT::Matrix<double> diff_pos(3,1);
      TNT::Matrix<double> inverted_orient(1,4);
      TNT::Matrix<double> translated_pos(3,1);

      diff_pos[0][0] = event.getPosition()[0]-antagonist->prop_event_pos[0][0];
      diff_pos[1][0] = event.getPosition()[1]-antagonist->prop_event_pos[1][0];
      diff_pos[2][0] = event.getPosition()[2]-antagonist->prop_event_pos[2][0];

      inverted_orient = invertOrientation(antagonist->prop_event_orient);
      translated_pos = calcRotationWithQuarternion(diff_pos, inverted_orient);

      TNT::Matrix<double> tmp(3,real_prop_point_list.num_cols()+1);
      for (int i=0; i<real_prop_point_list.num_cols();++i)
      {
         tmp[0][i] = real_prop_point_list[0][i];
         tmp[1][i] = real_prop_point_list[1][i];
         tmp[2][i] = real_prop_point_list[2][i];
      }

      tmp[0][real_prop_point_list.num_cols()] = translated_pos[0][0];
      tmp[1][real_prop_point_list.num_cols()] = translated_pos[1][0];
      tmp[2][real_prop_point_list.num_cols()] = translated_pos[2][0];
      real_prop_point_list = tmp;

      logPrintI("Point #%d stored.\n", real_prop_point_list.num_cols());
      if (real_prop_point_list.num_cols() >1)
      {
         int cols = real_prop_point_list.num_cols();
         TNT::Matrix<double> tmp(3,1);
         tmp[0][0] = real_prop_point_list[0][cols-1] - real_prop_point_list[0][cols-2];
         tmp[1][0] = real_prop_point_list[1][cols-1] - real_prop_point_list[1][cols-2];
         tmp[2][0] = real_prop_point_list[2][cols-1] - real_prop_point_list[2][cols-2];

         double len = sqrt(tmp[0][0]*tmp[0][0] + tmp[1][0]*tmp[1][0] + tmp[2][0]*tmp[2][0]);
         logPrintI("Distance between the last two points: %f\n", len);
      }

      ::Beep(825,250);
      if (real_prop_point_list.num_cols() == number_of_prop_points) // needed number of points stored
      {
         logPrintI("You now stored all needed points.\n");
         logPrintI("Use 'Backslash' to delete last point or 'c' to\n");
         logPrintI("calculate the result.\n");
      }
	  
   }

   //---------------------------------------------------------------------------
   //
   void PropRegistration::removeLastPoint()
   {
      if ( !real_prop_point_list.num_cols() == 0 )
      {
         TNT::Matrix<double> tmp(3,real_prop_point_list.num_cols()-1);
         for (int i=0; i<real_prop_point_list.num_cols()-1; ++i)
         {
             tmp[0][i] = real_prop_point_list[0][i];
             tmp[1][i] = real_prop_point_list[1][i];
             tmp[2][i] = real_prop_point_list[2][i];
         }
         real_prop_point_list = tmp;
         logPrintI("Point #%d deleted.\n", real_prop_point_list.num_cols()+1 );
      }
      else
         logPrintI("No points to delete.\n");
      ::Beep(825,250);
	  
   }

   //---------------------------------------------------------------------------
   //
   void PropRegistration::connectToMe(PropRegistration * antagonist_)
   {
      if (antagonist != 0 || antagonist_ == 0)
      {
         logPrintE("Fault: PropRegistration::connectToMe()\n");
         return;
      }
      antagonist = antagonist_;
      connected = true;
   }

   //---------------------------------------------------------------------------
   //
   void PropRegistration::calculateMeanError()
   {
      // 4x4 transformation matrix build of R and T
      TNT::Matrix<double> transform(4,4);  
      for (int i=0; i<3; ++i)
         for(int j=0; j<3; ++j)
            transform[i][j] = rotation[i][j];

      for (int i=0; i<3; ++i)
         transform[i][3] = translation[i][0];

      transform[3][3] = 1;

      //4xN matrix real_pointset (converted from 3xN)
      TNT::Matrix<double> real_prop_4(4,real_prop_point_list.num_cols());
      for (int i=0; i<real_prop_point_list.num_cols(); ++i)
      {
         real_prop_4[0][i] = real_prop_point_list[0][i];
         real_prop_4[1][i] = real_prop_point_list[1][i];
         real_prop_4[2][i] = real_prop_point_list[2][i];
         real_prop_4[3][i] = 1.0;
      }

      //4xN Matrix virtual_pointset (converted from 3xN)
      TNT::Matrix<double> virtual_prop_4(4,virtual_prop_point_list.num_cols());
      for (int i=0; i<virtual_prop_point_list.num_cols(); ++i)
      {
         virtual_prop_4[0][i] = virtual_prop_point_list[0][i];
         virtual_prop_4[1][i] = virtual_prop_point_list[1][i];
         virtual_prop_4[2][i] = virtual_prop_point_list[2][i];
         virtual_prop_4[3][i] = 1.0;
      }

      // re-transform matrix (= transform * virtual_pointset)
      TNT::Matrix<double> re_transform(4,virtual_prop_point_list.num_cols());
      re_transform = transform * virtual_prop_4;

      // difference matrix (= real_pointset - re-transform matrix)
      TNT::Matrix<double> difference(4,virtual_prop_point_list.num_cols());
      difference = real_prop_4 - re_transform;
         
      double sum = 0.0;
      double mean_sum = 0.0;

      //calculating vector with the single error values for each point 
      TNT::Matrix<double> error_vec(1,difference.num_cols());
      for(int i=0; i< difference.num_cols(); ++i)
      {
         double tmp = 0.0;
         tmp += difference[0][i] * difference[0][i];
         tmp += difference[1][i] * difference[1][i];
         tmp += difference[2][i] * difference[2][i];
         tmp = sqrt(tmp);
         error_vec[0][i] = tmp;
         sum += tmp;
      }

      mean_sum = sum / difference.num_cols();

      //claculate vector with the mean-square-error for each point
      TNT::Matrix<double> mean_error_vec(1,difference.num_cols());
      for(int i=0; i< mean_error_vec.num_cols(); ++i)
      {
         mean_error_vec[0][i] = error_vec[0][i] - mean_sum;
         mean_error_vec[0][i] *= mean_error_vec[0][i];
		 mean_error_vec[0][i] *= 0.01;
      }
      
      logPrintI("\n");
      for (int i=0; i<mean_error_vec.num_cols(); ++i)  
         logPrintI("MSE Point %d: %f \n", i+1, mean_error_vec[0][i]);
  
   }

   //---------------------------------------------------------------------------
   //
   void PropRegistration::modifyDTrackFile()
   {
    int number_of_points;
    TNT::Matrix<double> points;
    TNT::Matrix<double> modified_points;
    std::vector<char> text;

    if ( !readFromFile(number_of_points, points, text) )
       logPrintE("Error while reading from file: %s\n", d_track_file.c_str());

    modified_points.newsize(3,points.num_cols());

    for (int i=0; i<points.num_cols(); ++i)
    {
       modified_points[0][i] = points[0][i] + ((translation[0][0]*-1) + null_point[0][0]);
       modified_points[1][i] = points[1][i] + ((translation[1][0]*-1) + null_point[1][0]);
       modified_points[2][i] = points[2][i] + ((translation[2][0]*-1) + null_point[2][0]);
    }
    if ( !writeToFile(number_of_points, points, modified_points, text) )
       logPrintE("Error while writing to file: %s\n", d_track_file.c_str());
   }

   //---------------------------------------------------------------------------
   //
   bool PropRegistration::readFromFile(int & number_of_points, 
               TNT::Matrix<double> & points, std::vector<char> & text)
   {
    std::fstream * file;

    file = new std::fstream( d_track_file.c_str(), ios::in);
    if( file->good() )
    {
       file->setf( std::ios::skipws );
       file->clear();
       *file >> number_of_points;

       points.newsize(3,number_of_points);
       for (int i=0; i<number_of_points; ++i)
       {
          file->clear();
          *file >> points[0][i] >> points[1][i] >> points[2][i];
       }
       file->unsetf( std::ios::skipws );
       while ( !file->eof() )
       {
          char tmp_char;
          tmp_char = file->get();
          text.push_back(tmp_char);           
       }
       if (!text.empty())
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
   bool PropRegistration::writeToFile(int & number_of_points, TNT::Matrix<double> & points,
                           TNT::Matrix<double> & modified_points, std::vector<char> & text)
   {
    std::fstream * file;
    file = new std::fstream( d_track_file.c_str(), std::ios_base::out | std::ios_base::trunc );
    if( file->good() )
    {
       char str[100];
       sprintf(str, "%d\n", number_of_points);
	   *file << str << std::flush;

       for(int i=0; i<points.num_cols(); ++i)
       {
          sprintf(str, "%.10f %.10f %.10f\n",
               modified_points[0][i], 
               modified_points[1][i], 
               modified_points[2][i]);
		  *file << str << std::flush;
       }

       *file << std::endl << "# modified by PropRegistration.cxx" << std::endl << std::flush;
       *file << "# original values:" << std::endl << std::flush;
       for(int i=0; i<points.num_cols(); ++i)
       {
          sprintf(str, "# %.10f %.10f %.10f\n",
               points[0][i], 
               points[1][i], 
               points[2][i]);
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
   TNT::Matrix<double> PropRegistration::invertOrientation(TNT::Matrix<double> const & orientation)
   {
      TNT::Matrix<double> inverted(1,4);

      inverted[0][3] = orientation[0][3];
      for(int i=0; i<3; ++i)  // changes the direction of the orientation
         inverted[0][i] = orientation[0][i] * -1;  // [x, y, z, w] => [-x, -y, -z, w]

      return inverted;
   }

   //---------------------------------------------------------------------------
   //
   TNT::Matrix<double> PropRegistration::calcRotationWithQuarternion(TNT::Matrix<double> const point, TNT::Matrix<double> const orientation)
   {
      TNT::Matrix<double> rotated(3,1);
     
      //double w = orientation[0][0];
      double x = orientation[0][0];
      double y = orientation[0][1];
      double z = orientation[0][2];
      double w = orientation[0][3];

      rotated[0][0] = w*w*point[0][0] + 2*y*w*point[2][0] - 2*z*w*point[1][0] + x*x*point[0][0] + 2*y*x*point[1][0] + 2*z*x*point[2][0] - z*z*point[0][0] - y*y*point[0][0];
      rotated[1][0] = 2*x*y*point[0][0] + y*y*point[1][0] + 2*z*y*point[2][0] + 2*w*z*point[0][0] - z*z*point[1][0] + w*w*point[1][0] - 2*x*w*point[2][0] - x*x*point[1][0];
      rotated[2][0] = 2*x*z*point[0][0] + 2*y*z*point[1][0] + z*z*point[2][0] - 2*w*y*point[0][0] - y*y*point[2][0] + 2*w*x*point[1][0] - x*x*point[2][0] + w*w*point[2][0];

      return (rotated);
   }


} // namespace ot

