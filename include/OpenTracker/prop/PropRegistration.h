/** Header file for StylusCalibration node.
 *
 * @author Stefan Mooslechner
 */

/**
 * @page module_ref Module Reference
 * <p>
 * This Opentracker module is the second part of the Prop-Registration procedure. It provides you an
 * OpenTracker sink node which calculates a translation and a rotation matrix. Using these matrices,
 * the virtual prop will be congruent with the tracked real world prop. \n
 * If you provide a DTrack file, It is possible to cange the NullPoint to any position you specified 
 * at the virtual prop. \n
 * 
 * To register the prop, you need:
 * @li A calibrated stylus. 
 * @li A file containing the coordinates of the selected points of the virtual prop. (i.e. the output file 
 *       <tt>PropRegistration.exe</tt>.)
 * @li If you want to change the NullPoint of the D-Track file of the prop, you will need of course this file.
 * \n
 * </p>
 * <p>
 * Approach:
 * @li Set up the <tt>\<PropRegistrationSink\></tt>-tags in <tt>opentracker.xml</tt> with your 
 *    desired parameters (One tag for the stylus and one for the prop).
 * @li Start Opentracker (standalone or via Studierstube).
 * @li Touch the first point (the point which responds to the first point you marked on the virtual 
 *    prop) on the prop with the top of the stylus and press \<Page Up\> or \<Page Down\> to 
 *    store the point.
 *    (If you use a modified laser pointer as stylus, you usually have a \<Page Up\> and a 
 *    \<Page Down\> button on it. That is why we used these keys here.)
 * @li Do the same with all other points you further marked at the virtual prop. You have to 
 *    store the points in exactly the same order as you did at virtual prop.
 * @li You can remove the stored points in the opposite way around by pressing \<Backspace\>.
 * @li If all needed points are stored you get a message on the screen.
 * @li Now press \<c\> to start the calculation of the matrices. 
 * @li The calculated matrices are shown on the screen.
 * @li You can press \<ESC\> to interrupt the program at any time.
 * @li <b>NOTE: All keyboard inputs are only accepted if the stylus is visible for
 *    the tracking system. </b>
 * </p>
 *
 * <p>
 * Configure the module by using the configuration element 'PropRegistrationSink'.
 * You have to declare one sink for the stylus and one sink for the prop.
 * The following attributes are usable:
 * @li <tt> isStylus="true" | isProp="true" </tt> To declare the sink as stylus-sink or as prop-sink.
 * @li <tt> propfile="filename" </tt> Filename of the file build by the first part of the prop registration.
 *          If no propfile is given, the program can not be executed. (This element is ignored at a prop-node.)
 * @li <tt> dtrack="filename" </tt> Optional filename of the D-Track configuration file to modify.
 *          (This element is ignored at a prop-node.)
 * </p>
 * <p>
 * An example configuration element for the stylus-node looks like this :
 * @verbatim
   <PropRegistrationSink isStylus="true" propfile="propPoints.txt" dtrack="2modify.txt">
      <SomeEventSource />
   </PropRegistrationSink> @endverbatim
 * </p>
 * <p>
 * An example configuration element for the prop-node looks like this :
 * @verbatim
   <PropRegistrationSink isProp="true">
      <SomeEventSource />
   </PropRegistrationSink> @endverbatim
 * </p>
 * <p>
 * Hint for the settings in the <tt>opentracker.xml \<ConsoleConfig\></tt>-tag:
 * @li Make sure that the display-setting is <b>"off"</b>. Otherwise, you are not able to 
 *     read the informations and instructions on the console window.
 * @verbatim
   <ConsoleConfig display="off" /> @endverbatim
 * </p>
 */



#ifndef _PROP_REGISTRATION_H
#define _PROP_REGISTRATION_H

#include "../OpenTracker.h"
#include "OpenTracker/prop/tnt/tnt_cmat.h"


namespace ot {

   class OPENTRACKER_API PropRegistration
   {     
      friend class PropRegistrationModule;
      friend class PropRegistrationSink;


// members for both nodes (stylus and prop):
   protected:
      /** Flag if this module is stylus-object or prop-object. */
      bool is_stylus;        
       
   private:
   

   
      /** Pointer to the second object of the stylus/prop-pair. */
      PropRegistration * antagonist;

      /** Flag if the stylus-prop-pair is already connected. */
      bool connected;
       
// members for stylus node:   

      /** Filename of the DTrackfile to modyfie. 
       *  \n(Member just used by stylus-node.) */
      std::string d_track_file;

      /** Filename of the file with the points of the virtual prop.
       *  (i.e. the outputfile of PropRegistration part 1) 
       *  \n(Member just used by stylus-node.) */
      std::string prop_file;

      /** Matrix stores the points of the real world prop. 
       *  \n(Member just used by stylus-node.) */
      TNT::Matrix<double> real_prop_point_list;

      /** Number of points for the calculation. (The number of points
       *  is read from 'prop_file'.) 
       *  \n(Member just used by stylus-node.) */
      int number_of_prop_points;

      /** Matrix stores the points of the virtual prop. (This points are
       *  read from 'prop_file'.)
       *  \n(Member just used by stylus-node.) */
      TNT::Matrix<double> virtual_prop_point_list;

      /** Flag if the nullpoint of the prop has to be changed.
       *  \n(Member just used by stylus-node.) */
      bool change_null_point;

      /** Translationvector(3x1) stores the result of the calcualtion.
       *  \n(Member just used by stylus-node.) */
      TNT::Matrix<double> null_point;

      /** Rotationmatrix(3x3) stores the result of the calcualtion.
       *  \n(Member just used by stylus-node.) */
      TNT::Matrix<double> rotation;

      /** Translationvector(3x1) stores the result of the calcualtion.
       *  \n(Member just used by stylus-node.) */
      TNT::Matrix<double> translation;
       
// members for prop node:
      /** Vector(3x1) to store the actual position of the prop. 
       *  \n(Member just used by prop-node.) */
      TNT::Matrix<double> prop_event_pos;

      /** Vector(1x4) to store the actual orientation of the prop. 
       *  \n(Member just used by prop-node.) */
      TNT::Matrix<double> prop_event_orient;

      /** Time of the last stored event.
       *  \n(Member just used by prop-node.) */
      double prop_event_time;
       
       
	           
// methods for both nodes (stylus and prop):   
   protected:
      /** Constructor method
       *  @param is_stylus_ if TRUE the node is a stylus-node, else it is a prop-node.
       *  @param antagonist_ pointer to the opponent node if it already exists.
       *  @param prop_file_ name of the file with the stored points of the virtual prop.
       *  @param d_track_file_ name of the DTrack-file to modyfie.*/
      PropRegistration(bool is_stylus_, PropRegistration * antagonist_, 
         std::string prop_file_, std::string d_track_file_);
       
    private:
      /** Copyconstructor method*/
      PropRegistration(PropRegistration &src){}

      /** Destructor method*/
      virtual ~PropRegistration(){}
      
      /** Initializes the node depending of the node type. */
      void init();

      /** Allowes the opponent node to connect to this node.
       *  @param antagonist_ self-pointer */
      void connectToMe(PropRegistration * antagonist_);

      /** Depending of the node-type it stores the actual position and orientation (prop-node)
       *  or it looks for keyboard input.*/
      void handleEvent( Event & event);

      /** Stopps the  program. */
      void allFinished();

	  /* show recorded points in iv for debug purposes */
	  void showPoints (int & number_of_points, TNT::Matrix<double> & points, const char *text);

// here starts judith's construction area
	  /* write the equation to solve into a text file for debug reasons */
	  //void writeEquation(TNT::Matrix<double> &realPoints,TNT::Matrix<double> &virtualPoints, const char *filename);

	  /* Gram-Schmidt-Orthogonalization to compute QR Factorization. Take a look at Strang "Linear Algebra and its 
	     Applications, p. 172 ff. */
	  //void gramSchmidt(TNT::Matrix<double> &aMatrix, TNT::Matrix<double> &qMatrix, TNT::Matrix<double> &rMatrix, 
		//  int, int);

	  /* Gauss-Jordan to invert upper triangular 3x3 matrix */
	  //void gaussJordan(TNT::Matrix<double> &a,TNT::Matrix<double> &inverted );

	  /* write 3x3 Matrix to file*/
	  

	  

	  
	  
	  void writeIVTransform();
	  
	  void calculateResultObsolete();

    void calculateResultNewVersion();
    void moveToCenterOfMass(TNT::Matrix<double> &real_prop_center_of_mass,
                            TNT::Matrix<double> &virtual_prop_center_of_mass);
    void calculateSVD();
    void multiplyMxN(TNT::Matrix<double> &leftMa, TNT::Matrix<double> &rightMa, TNT::Matrix<double> &resultMa);
    void writeMatrix (TNT::Matrix<double> &, const char *);
    void transpose (TNT::Matrix<double> &in, TNT::Matrix<double> &out);
// end of judith's mess

// methods for stylus node: 
      /** Loads the point from 'prop_file' and stores them to virtual_prop_point_list. 
       *  \n(Method just used by stylus-node.) */ 
      bool loadPropFile();

      /** Checks and handles the keyboard input. 
       *  \n(Method just used by stylus-node.) */
      void checkKeyboard(Event & event);

      /** Stores a new point to real_prop_point_list. 
       *  \n(Method just used by stylus-node.) */
      void storePoint(Event & event);

      /** Removes the last stored point from real_prop_point_list. 
       *  \n(Method just used by stylus-node.) */
      void removeLastPoint();

      /** Calculates the rotation and translation between the two given pointsets. 
       *  \n(Method just used by stylus-node.) */
      void calculateResult();

      /** Checks if the DTrack-filename stored in d_track_file is valid.
       *  \n(Method just used by stylus-node.)
       *  @return TRUE if the file exists, FALSE else.*/
      bool checkIfDTrackFileExists();

      /** Modifies the DTrack-file, so that the null-point is in the given point. 
       *  \n(Method just used by stylus-node.) */
      void modifyDTrackFile();

      /** Helping function for modifyDTrackFile() which reads the stored
       *  points form the file. 
       *  \n(Method just used by stylus-node.)
       *  @param number_of_points reference to an int to store the number of points 
       *  @param points reference to a matrix(3xN) to store the points
       *  @param text reference to a \<char\>vector to store the remaining text from the file
       *  @return TRUE if reading was succesfull, FALSE else. */
      bool readFromFile(int & number_of_points, 
            TNT::Matrix<double> & points, std::vector<char> & text);

      /** Helping function for modifyDTrackFile() which writes the new
       *  calculated points back to the file.
       *  \n(Method just used by stylus-node.)
       *  @param number_of_points reference to an int were the number of points is stored
       *  @param points reference to a matrix(3xN) were the original points to write are stored
       *  @param modified_points reference to a matrix(3xN) were the new calculated points to write are stored
       *  @param text reference to a \<char\>vector were the remaining text from the file is stored
       *  @return TRUE if writing was succesfull, FALSE else. */
      bool writeToFile(int & number_of_points, TNT::Matrix<double> & points,
                        TNT::Matrix<double> & modified_points, std::vector<char> & text);

      /** Calculates the mean error for every point of the prop and shows the result on 
       *  the screen. 
       *  \n(Method just used by stylus-node.) */
      void calculateMeanError();

      /** Changes the direction of the rotation of th given quaternion to the opposite site.
       *  (i.e. [x, y, z, w] -> [-x, -y, -z, w]) 
       *  \n(Method just used by stylus-node.)
       *  @param orientation refernece to a quaternion represented by a 1x4 matrix
       *  @return the opposite of the given quaternion */
      TNT::Matrix<double> invertOrientation(TNT::Matrix<double> const & orientation);

      /** Calculates the result of a rotation specified by a qaternion.
       *  \n(Method just used by stylus-node.)
       *  @param point the point to rotate (3x1 matrix)
       *  @param orientation the quaternion (1x4 matrix)
       *  @return the rotated point */
      TNT::Matrix<double> calcRotationWithQuarternion (TNT::Matrix<double> const point, 
                        TNT::Matrix<double> const orientation);

// methods for prop node: 


   }; // class PropRegistration

} // namespace ot

#endif //_PROP_REGISTRATION_H