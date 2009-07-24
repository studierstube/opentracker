#ifndef OT_NO_PROPREGISTRATIONMOUDLE_SUPPORT
#include <OpenTracker/prop/PropRegistration.h>
#include <OpenTracker/prop/rigidTransform/RigidTransform.h>

#include <ace/Log_Msg.h>
#include <fstream>

#include "OpenTracker/svd/svd.h"

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
         //logPrintI("   -> DTrack file to modify: %s\n",d_track_file.c_str());
      }
      else
      {
         logPrintI("   -> Node is prop\n");
         logPrintI("   -> DTrack file to modify: %s\n",d_track_file.c_str());
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
 
   void PropRegistration::calculateResult()      
   {
 
      if (real_prop_point_list.num_cols() != number_of_prop_points)
      {
         logPrintW("The number of stored points differs to the number of needed points.\n");
         logPrintW("Unable to start the calculation.\n");
         return;
      }
    
      writeMatrix(real_prop_point_list, "pointlists/real_points.txt");
      writeMatrix(virtual_prop_point_list, "pointlists/virtual_points.txt");

      
      calculateResultNewVersion();
      //calculateResultObsolete();
      
           
      writeIVTransform();     
      calculateMeanError();
      
      //modifyDTrackFile();
      allFinished();
   }
   
   //---------------------------------------------------------------------------
   //--------------------------------------------------------------------------- 
   void PropRegistration::calculateResultObsolete()
   {
     RigidTransform transform_calculator(virtual_prop_point_list,real_prop_point_list);
     rotation.newsize(3,3);
     rotation = transform_calculator.getR();
     translation.newsize(3,1);
     translation = transform_calculator.getT();
   }
  
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
   void PropRegistration::moveToCenterOfMass(
      TNT::Matrix<double> &real_c_m, 
      TNT::Matrix<double> &virtual_c_m)
   {
     double average0=0;
     double average1=0;
     double average2=0;
     for(int i=0; i<real_prop_point_list.num_cols();i++)
     {
       average0+=real_prop_point_list[0][i];
       average1+=real_prop_point_list[1][i];
       average2+=real_prop_point_list[2][i];
     }
     TNT::Matrix<double> realPropCenterOfMass(3,1);
     realPropCenterOfMass[0][0]= average0/real_prop_point_list.num_cols();
     realPropCenterOfMass[1][0]= average1/real_prop_point_list.num_cols();
     realPropCenterOfMass[2][0]= average2/real_prop_point_list.num_cols();

     // for all rows subtract respective average
     for(int i=0; i<real_prop_point_list.num_cols();i++)
     {
       real_prop_point_list[0][i]-=realPropCenterOfMass[0][0];
       real_prop_point_list[1][i]-=realPropCenterOfMass[1][0];
       real_prop_point_list[2][i]-=realPropCenterOfMass[2][0];
     }

     
     // same for virtual point list
     TNT::Matrix<double> virtualPropCenterOfMass(3,1);

     average0=0;
     average1=0;
     average2=0;
     for(int i=0; i<virtual_prop_point_list.num_cols();i++)
     {
       average0+=virtual_prop_point_list[0][i];
       average1+=virtual_prop_point_list[1][i];
       average2+=virtual_prop_point_list[2][i];
     }
     virtualPropCenterOfMass[0][0]=average0/virtual_prop_point_list.num_cols();
     virtualPropCenterOfMass[1][0]=average1/virtual_prop_point_list.num_cols();
     virtualPropCenterOfMass[2][0]=average2/virtual_prop_point_list.num_cols();
     // for all rows subtract respective average
     for(int i=0; i<virtual_prop_point_list.num_cols();i++)
     {
       virtual_prop_point_list[0][i]-=virtualPropCenterOfMass[0][0];
       virtual_prop_point_list[1][i]-=virtualPropCenterOfMass[1][0];
       virtual_prop_point_list[2][i]-=virtualPropCenterOfMass[2][0];
     }

     //writeEquation(virtual_prop_point_list, virtualPropCenterOfMass, "virtualCM.txt");
     writeMatrix(realPropCenterOfMass,"pointlists/real_center_mass.txt");
     writeMatrix(virtualPropCenterOfMass,"pointlists/virtual_center_mass.txt");

     real_c_m = realPropCenterOfMass;
     virtual_c_m = virtualPropCenterOfMass;
   }
  
  
  //---------------------------------------------------------------------------
  //---------------------------------------------------------------------------
  void PropRegistration::calculateResultNewVersion()
  {
    TNT::Matrix<double> real_prop_center_of_mass(3,1);
    TNT::Matrix<double> virtual_prop_center_of_mass(3,1);
     
    moveToCenterOfMass(real_prop_center_of_mass, virtual_prop_center_of_mass);
    
    //correlating the corresponding point's products
    TNT::Matrix<double> correlation_H(3,3);
    TNT::Matrix<double> additive(3,3);
    additive = 0;
    
    for(int i = 1 ; i <= real_prop_point_list.num_cols() ; i++)
    {
      TNT::Matrix<double> real_cur_vec(3,1);
      TNT::Matrix<double> virt_cur_vec(1,3);
      
      real_cur_vec(1,1) = real_prop_point_list(1,i);
      real_cur_vec(2,1) = real_prop_point_list(2,i);
      real_cur_vec(3,1) = real_prop_point_list(3,i);
      
      virt_cur_vec(1,1) = virtual_prop_point_list(1,i);
      virt_cur_vec(1,2) = virtual_prop_point_list(2,i);
      virt_cur_vec(1,3) = virtual_prop_point_list(3,i);
      
      multiplyMxN(real_cur_vec, virt_cur_vec, additive);
      correlation_H = correlation_H + additive;
    }
    
    
    //now calculating the svd of the correlation matrix
    //first, converting the matrix to be decomposed into
    //a 2d array according to the AP - namespace
    ap::real_2d_array correlation_H_svd;
    correlation_H_svd.setbounds(0,2,0,2);
    for(int i = 1 ; i <= 3 ; i++){
      for(int j = 1 ; j <= 3 ; j++){
        correlation_H_svd(j-1, i-1) = correlation_H(j,i);
      }
    }
    
    //initializing the return-matrices of svd:
    ap::real_2d_array svd_u;
    svd_u.setbounds(0,2,0,2);
    ap::real_2d_array svd_v_t;
    svd_v_t.setbounds(0,2,0,2);
    ap::real_1d_array svd_sigma;
    svd_sigma.setbounds(0,2);
    int num_rows = correlation_H.num_rows();
    int num_cols = correlation_H.num_cols();
    rmatrixsvd(correlation_H_svd,
               num_rows,
               num_cols,
               2,2,2,
               svd_sigma,
               svd_u,
               svd_v_t);
    
    //reconversion from ap-namespace to tnt matrices:
    TNT::Matrix<double> tnt_svd_u(3,3);
    TNT::Matrix<double> tnt_svd_u_t(3,3);
    TNT::Matrix<double> tnt_svd_v(3,3);
    TNT::Matrix<double> tnt_svd_v_t(3,3);
    
    for(int i = 1 ; i <= 3 ; i++){
      for(int j = 1 ; j <= 3 ; j++){
        tnt_svd_u(j,i) = svd_u(j-1,i-1);
        tnt_svd_v_t(j,i) = svd_v_t(j-1,i-1);
      }
    }
    
    //transposing the two matrices from the svd
    //to be able to calculate : rot = v * u^t
    transpose(tnt_svd_u, tnt_svd_u_t);   
    transpose(tnt_svd_v_t, tnt_svd_v);   
    
    //and finally calculating the rotation:
    rotation.newsize(3,3);
    multiplyMxN(tnt_svd_v, tnt_svd_u_t, rotation);
    
    double det_r = rotation(1,1) * ( rotation(2,2) * rotation(3,3) - rotation(2,3) * rotation(3,1) )
                 - rotation(2,1) * ( rotation(1,2) * rotation(3,3) - rotation(1,3) * rotation(3,2) )
                 + rotation(3,1) * ( rotation(1,2) * rotation(2,3) - rotation(1,3) * rotation(2,2) );
    
    
    //calculating the translation:
    translation.newsize(3,1);
    translation = real_prop_center_of_mass + virtual_prop_center_of_mass;    
    
    
   }        
 
  //---------------------------------------------------------------------------
  //---------------------------------------------------------------------------
   void PropRegistration::writeIVTransform()
   {
     if (change_null_point) // modify DTrack file and we need no translation in the .iv file
     {
       //modifyDTrackFile();
       logPrintI("DTrack file was modified. The following translation was added: \n");
       logPrintI("%f %f %f \n", ((translation[0][0]*-1) /*+ null_point[0][0]*/)
         , ((translation[1][0]*-1) /*+ null_point[1][0]*/)
         , ((translation[2][0]*-1) /*+ null_point[2][0])*/));
     }

     logPrintI("You can find the additional transform for the \n"
       "Inventor-File of the prop in the file \"iv_addition.txt\"\n");
     std::fstream out_stream;
     out_stream.open("iv_addition.txt",std::ios::out);
     out_stream.clear();
     out_stream << "Transform {" << std::endl;
     out_stream << "  translation " << translation[0][0] << " " << translation[1][0] << " " << translation[2][0] << std::endl;
     out_stream << "}" << std::endl << std::endl;
   
     out_stream << "MatrixTransform {" << std::endl;
     out_stream << "matrix ";
     out_stream << rotation[0][0] << " " << rotation[0][1] << " " << rotation[0][2] << " 0" << std::endl;
     out_stream << rotation[1][0] << " " << rotation[1][1] << " " << rotation[1][2] << " 0" << std::endl;
     out_stream << rotation[2][0] << " " << rotation[2][1] << " " << rotation[2][2] << " 0" << std::endl;
     out_stream << "0 0 0 1" << std::endl;
     out_stream << "}" << std::endl << std::endl;
   }
   
   
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   void PropRegistration::storePoint(Event & event)
   {    
      if (antagonist ==  0)
      {
         logPrintE("PropRegistration::storePoint() - No antagonist -> No comparison\n");
         return;
      }

      if (real_prop_point_list.num_cols() == number_of_prop_points) // needed number of points already stored
      {
        logPrintW("You already stored all needed points.\n");
        logPrintI("Use 'Backslash' to delete points or 'c' to\n");
        logPrintI("calculate the result.\n");
 		    showPoints(number_of_prop_points,real_prop_point_list,"debug.iv");
        return;
      }         

      double time_diff = event.time - antagonist->prop_event_time;
       if (time_diff > 250.0)  //if the stored point is 'older' than a quarter second
      {
         logPrintW("Time difference between the two events was to long.\n");
         logPrintW("Maybe the prop is not visible?\n");
         logPrintW("Point not stored!\n");
         return;
      }

      if(is_stylus)
      {
         //now, get the point picked by the stylus:
         TNT::Matrix<double> stylus_pos(3,1);
         stylus_pos(1,1) = event.getPosition()[0];
         stylus_pos(2,1) = event.getPosition()[1];
         stylus_pos(3,1) = event.getPosition()[2];
         writeMatrix(stylus_pos,"debug/stylus_pos.txt");            
         //next, get position and orientation of the prop:
         TNT::Matrix<double> prop_trans(3,1);
         prop_trans = antagonist->prop_event_pos;
         writeMatrix(prop_trans,"debug/prop_trans.txt");      
         //now, calculate the position of the stylus point
         //relative to the null point of the prop
         //yielding a vector in prop coordinates
         //that is rotated in world coordinates
         TNT::Matrix<double> relative_pos(3,1);
         relative_pos(1,1) = stylus_pos(1,1) - prop_trans(1,1);
         relative_pos(2,1) = stylus_pos(2,1) - prop_trans(2,1);
         relative_pos(3,1) = stylus_pos(3,1) - prop_trans(3,1);
         writeMatrix(relative_pos,"debug/relative_pos.txt");            

         //rotate vector back to world coordinates
         TNT::Matrix<double> prop_rotation(1,4);
         prop_rotation = antagonist->prop_event_orient;
         double b = prop_rotation(1,1);
         double c = prop_rotation(1,2);
         double d = prop_rotation(1,3);
         double a = prop_rotation(1,4);
          
         //just for safety - normalize the quaternion
         double length = sqrt(a*a + b*b + c*c + d*d);
         a /= length;
         b /= length;
         c /= length;
         d /= length;

         //rotation matrix from quaternion
         //quaternion definition: a + b*i + c*j + d*k
         //OT quaternion is (b,c,d,a)
         TNT::Matrix<double> mat_prop_rotation(3,3);
         mat_prop_rotation(1,1) = a*a + b*b - c*c - d*d;
         mat_prop_rotation(2,1) = 2*a*d + 2*b*c;
         mat_prop_rotation(3,1) = 2*b*d - 2*a*c;

         mat_prop_rotation(1,2) = 2*b*c - 2*a*d;
         mat_prop_rotation(2,2) = a*a - b*b + c*c - d*d;
         mat_prop_rotation(3,2) = 2*a*b + 2*c*d;

         mat_prop_rotation(1,3) = 2*a*c + 2*b*d;
         mat_prop_rotation(2,3) = 2*c*d - 2*a*b;
         mat_prop_rotation(3,3) = a*a - b*b - c*c + d*d;

         writeMatrix(mat_prop_rotation,"debug/mat_prop_rotation.txt");      
         //transposed matrix is at the same time inverted matrix      
         TNT::Matrix<double> mat_prop_rotation_inverse(3,3);
         transpose(mat_prop_rotation, mat_prop_rotation_inverse);
         writeMatrix(mat_prop_rotation_inverse,"debug/mat_prop_rotation_inverse.txt");      

         TNT::Matrix<double> final_vector(3,1);
         multiplyMxN(mat_prop_rotation_inverse, relative_pos, final_vector);
         writeMatrix(final_vector,"debug/final_vector.txt");            

         //store the new vector
         TNT::Matrix<double> temp_point_list(3,real_prop_point_list.num_cols()+1);
         for (int i=0; i<real_prop_point_list.num_cols();++i)
         {
           temp_point_list[0][i] = real_prop_point_list[0][i];
           temp_point_list[1][i] = real_prop_point_list[1][i];
           temp_point_list[2][i] = real_prop_point_list[2][i];
         }

         temp_point_list[0][real_prop_point_list.num_cols()] = final_vector(1,1); 
         temp_point_list[1][real_prop_point_list.num_cols()] = final_vector(2,1); 
         temp_point_list[2][real_prop_point_list.num_cols()] = final_vector(3,1); 
         real_prop_point_list = temp_point_list;
         /*
         temp_point_list[0][real_prop_point_list.num_cols()] = relative_pos(1,1); 
         temp_point_list[1][real_prop_point_list.num_cols()] = relative_pos(2,1); 
         temp_point_list[2][real_prop_point_list.num_cols()] = relative_pos(3,1); 
         real_prop_point_list = temp_point_list;
         */

         
         logPrintI("Point #%d stored.\n", real_prop_point_list.num_cols());
         
         /*
         logPrintI("Stylus     x: %3f   y: %3f   z: %3f\n",
            stylus_pos(1,1),
            stylus_pos(2,1),
            stylus_pos(3,1));
         logPrintI("Probe      x: %3f   y: %3f   z: %3f\n",
            prop_trans(1,1),
            prop_trans(2,1),
            prop_trans(3,1));
         logPrintI("Relative:  x: %3f   y: %3f   z: %3f\n",
            relative_pos(1,1),
            relative_pos(2,1),
            relative_pos(3,1));


         logPrintI("Rotated:   x: %3f   y: %3f   z: %3f\n",
            final_vector(1,1),
            final_vector(2,1),
            final_vector(3,1));

         */

         
         // compute the distance between two subsequent recordings
         if (real_prop_point_list.num_cols() >1)
         {

           int cols = real_prop_point_list.num_cols();
           TNT::Matrix<double> tmp(3,1);
           tmp(1,1) = real_prop_point_list(1,cols-1) - real_prop_point_list(1,cols);
           tmp(2,1) = real_prop_point_list(2,cols-1) - real_prop_point_list(2,cols);
           tmp(3,1) = real_prop_point_list(3,cols-1) - real_prop_point_list(3,cols);

           double len = sqrt(tmp(1,1)*tmp(1,1) + tmp(2,1)*tmp(2,1) + tmp(3,1)*tmp(3,1));
           logPrintI("Distance between the last two points: %f\n", len);
         }

         //::Beep(825,250);
         if (real_prop_point_list.num_cols() == number_of_prop_points) // needed number of points stored
         {
           logPrintI("You now stored all needed points.\n");
           logPrintI("Use 'Backslash' to delete last point or 'c' to\n");
           logPrintI("calculate the result.\n");
         }

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

         
         
      std::fstream out_stream;
      out_stream.open("mean_error.txt",std::ios::out);
      out_stream.clear();
      for (int i=0; i<mean_error_vec.num_cols(); ++i){
        out_stream << "Point nr. " << i << " :  " << mean_error_vec[0][i] << std::endl;
      }
      out_stream.close();
  
   }

   //---------------------------------------------------------------------------
   //
   void PropRegistration::modifyDTrackFile()
   {
    if(is_stylus && !antagonist->is_stylus)
    {
      int number_of_points;
      TNT::Matrix<double> points;
      TNT::Matrix<double> modified_points;
      std::vector<char> text;
      modified_points.newsize(points.num_rows(), points.num_cols());
      
      if ( !antagonist->readFromFile(number_of_points, points, text) )
         logPrintE("Error while reading from file: %s\n", d_track_file.c_str());
      
      for (int i=0; i<points.num_cols(); ++i)
      {
         modified_points[0][i] = points[0][i] + translation[0][0];
         modified_points[1][i] = points[1][i] + translation[1][0];
         modified_points[2][i] = points[2][i] + translation[2][0];
      }
      if ( !antagonist->writeToFile(number_of_points, points, modified_points, text) )
         logPrintE("Error while writing to file: %s\n", antagonist->d_track_file.c_str());
         
      
     }
     
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

	  
void PropRegistration::showPoints (int & number_of_points, TNT::Matrix<double> & points, const char *text)
{
	std::ofstream file;
	
	file.open(text);
	file << "#Inventor V2.0 ascii\n\nSeparator {\n";
	file << "Separator { SoMaterial { diffuseColor 1 1 1 } Sphere {radius 5} } # origin\n";
	file << "Separator { Transform { translation 10 0 0 } SoMaterial { diffuseColor 1 0 0 } Sphere {radius 5} } # origin\n";
	file << "Separator { Transform { translation 0 10 0 } SoMaterial { diffuseColor 0 1 0 } Sphere {radius 5} } # origin\n";
	file << "Separator { Transform { translation 0 0 10 } SoMaterial { diffuseColor 0 0 1 } Sphere {radius 5} } # origin\n";

	for (int i=0;i<number_of_points; i++)
	  file << "Separator { Transform { translation " << points[0][i] << " " << points[1][i] << " " << points[2][i]
           << "} Sphere { radius 2 } }\n";
    file << "}\n";
	file.close();
	return;
}

void PropRegistration::writeMatrix (TNT::Matrix<double> &mat, const char *filename)
{
	std::ofstream file;
	int i;

	file.open(filename);
	file << "Matrix: \n";


  for(int row = 1 ; row <= mat.num_rows() ; row++){
	  for(int col = 1 ; col <= mat.num_cols() ; col++){
	    file << mat(row,col) << " ";
	  }
	  file << "\n";
	}
	
		
	
	file.close();
	return;
}


void PropRegistration::transpose (TNT::Matrix<double> &in, TNT::Matrix<double> &out)
{
	int i,j;

	for (i=0;i<in.num_cols();i++)
		for (j=0;j<in.num_rows();j++)
			out[i][j]=in[j][i];
	
	return;

} 

void PropRegistration::multiplyMxN(TNT::Matrix<double> &leftMa, TNT::Matrix<double> &rightMa, TNT::Matrix<double> &resultMa)
{
	int i,j,k;

	for (i=0;i<resultMa.num_rows();i++)
		for (j=0;j<resultMa.num_cols();j++)
		{
			resultMa[i][j]=0;
			for (k=0;k<leftMa.num_cols();k++)
				resultMa[i][j]+=leftMa[i][k]*rightMa[k][j];
		}

	return;
}
   //---------------------------------------------------------------------------
   //
   void PropRegistration::checkKeyboard(Event & event)
   {
      int key = 0;
      #ifdef WIN32
	    ::Sleep(15); 
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


} // namespace ot




#endif //OT_NO_PROPREGISTRATIONMOUDLE_SUPPORT







