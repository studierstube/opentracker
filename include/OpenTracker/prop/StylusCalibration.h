
/** Header file for StylusCalibration node.
 *
 * @author Stefan Mooslechner
 */

/**
 * @page module_ref Module Reference
 * <p>
 * This Opentracker module is used to calibrate a stylus. The goal is to calculate
 * the top of the stylus as accurately as possible. Therefore, we fix the 
 * top, move the stylus around and store the tracked points.
 * These points are, depending on the quality of the tracking source, more or less
 * located on a hemisphere. Via this hemisphere, it is possible to calculate 
 * the center of it, which is equal to the top of the stylus.\n
 * The result of the calculation is a 3D vector which describes the offset
 * from the Tracking-NullPoint to the top of the stylus.\n
 * It is also possible to modify the D-Track configuration file in a way, that the
 * NullPoint is set to the top of the stylus. Normally, the NullPoint is in one of
 * the tracked spheres. So, with this modified file, the stylus can be
 * used without further considering the offset, and the tracked coordinate
 * provided to you by the ART-Tracker is on the top of the stylus.
 * </p>
 * <p>
 * <b>Approach:</b>\n
 * @li Set up the <tt>\<StylusSink\></tt>-tag in <tt>opentracker.xml</tt> with your 
 *    desired parameters.
 * @li Start Opentracker (standalone or via Studierstube).
 * @li If the module receives data from your stylus, you will be asked 
 *    to hold your stylus vertical and press \<Page Up\> or \<v\> to contiue.\n
 *    (If you use a modified laser pointer as stylus, you usually have a \<Page Up\> and a 
 *    \<Page Down\> button on it. That is why we used these keys here.)
 * @li Now you are asked to move the stylus aboute 45 degrees in any direction and press
 *    \<Page Down\> or \<c\>.
 * @li Now the first step of the calibration procedure is finished. You are asked to move
 *    the stylus around. Meanwhile, the tracked points are stored for further usage.
 * @li There must be a minimum distance between the single points to be used for the 
 *    calculation. So, try to scan the area of the hemisphere as largely as possible.
 * @li In the console window, you will be informed aboute the current number of valid
 *    points.
 * @li If the needed number of samples is stored, the calculation starts automatically and
 *    the result is written on the console window.
 * @li Depending on your settings, a scatter plot of the tracked points is shown, or/and
 *    a textfile with the tracked points and the result is written.
 * @li If you name a D-Track configuration file, it will be modified.
 * @li You can press \<ESC\> to interrupt the program any time.
 * @li <b>NOTE: All keyboard inputs are only accepted if the stylus is trackable for
 *    the tracking system. </b>
 * </p>
 * <p>
 * Configure the module by using the configuration element 'StylusSink'. Following attributes
 * (with their default values) are usable:
 * @li <tt> samples(n) [500]</tt>  The number of samples taken for the calculation.
 * @li <tt> showScatter(true|false) [false] </tt>  Flag for showing a scatter plot after finishing the calculation.
 *  (Studierstube must be available on your system, and you have to provide the necessary files as described at
 *  the end of this page.)
 * @li <tt> saveData(true|false) [false]</tt>  Flag for saving a plaintextfile with the samples and the result.
 * @li <tt> file('filename') ['no file']</tt> Filename for the DTrack configuration file to modify.
 * </p>
 * <p>
 * An example for a configuration element looks like this :
 * @verbatim
   <StylusSink samples="400" showScatter="true" saveData="true" file="2modify.txt">
      <SomeEventSource />
   </StylusSink> @endverbatim
 * </p>
 * <p>
 * Hint for the settings in the <tt>opentracker.xml \<ConsoleConfig\></tt>-tag:
 * @li Make sure that the display-setting is <b>"off"</b>. Otherwise, you are not able to 
 *     read the informations and instructions on the console window.
 * @verbatim
   <ConsoleConfig display="off" /> @endverbatim
 * </p>
 * <p> \n </p>
 * <p>
 * Needed structures for showing the scatter plot for the samples:
 * @li subfolder named <tt>plot</tt> containing the following files:
 * @li <tt>kernel.xml</tt>
 * @li <tt>showplot.bat</tt>
 * @li <tt>viewer.iv</tt>
 * (Just copy and paste the content of to build the files.)
 * </p>
 * <p>
 * Content of <tt>kernel.xml</tt>: 
 * @verbatim
 <studierstube>
   <kernel updateMode="timer" logMode="console" updateRate="0.03333"/>
   <Component name="Viewer" availability="onload" lib="stbviewer_win">
      <Param key="configFile" value="viewer.iv" />
   </Component>
   <Application name="SimpleApp" availability="onload" lib="simpleApp">
      <Param key="sceneFile"  value="scatter.iv" />
      <Param key="needViewer" value="true" />	
   </Application>
 </studierstube> @endverbatim
 * \n
 * Content of <tt>showplot.bat</tt>: 
 * @verbatim
 cd plot
 studierstube.exe @endverbatim
 * \n
 * Content of <tt>viewer.iv</tt>: 
 * @verbatim
 #Inventor V2.1 ascii
 SoDisplay 
 {
    sceneGraph SoSeparator{
       SoStbScene{}  
    }
 } @endverbatim
 * </p>
 */


#ifndef _STYLUS_CALIBRATION_H
#define _STYLUS_CALIBRATION_H

#include "../OpenTracker.h"

#include <string>
#include <vector>

#include <OpenTracker/prop/tnt/tnt_array2d.h>
#include <OpenTracker/prop/tnt/tnt_cmat.h>

namespace ot {

    class OPENTRACKER_API StylusCalibration
    {

       /** Standard value for the number of samples used for the calculation. */
#define STANDARD_NUMBER_OF_SAMPLES 500
       
       friend class StylusModule;
       friend class StylusSink;

       //Members:
    protected :
       /** Filename for the file to modify (ART-config file) */
       const std::string filename;

       /** Flag for showing the scatterplot after calculation. */
       bool show_scatter;

       /** Flag for saving the result and the samples as plaintext in 'result.txt'.*/
       bool save_result;

       /** Numbers of samples taken for the calculation. */
       unsigned int number_of_samples;

       /** Smallest distance between two samples to be considered for
        *  the calculation. */
       float minimal_distance;

       /** First point (3x1 matrix) to calculate the minimal distance. */
       TNT::Matrix<double> init_point_a;

       /** Second point (3x1 matrix) to calculate the minimal distance. */
       TNT::Matrix<double> init_point_b;

       /** Stores all the translation samples (as 3x1 matrices) for the calculation. */
       std::vector<TNT::Matrix<double>> translation_samples;

       /** Stores all the orientation samples (as 1x4 matrices) for the calculation. */
       std::vector<TNT::Matrix<double>> orientation_samples;

       /** mean sample (as 3x1 matrix) */
       TNT::Matrix<double> mean_sample;

       // result of the sphere-fitting-calculation
       //SbVec3f result_sphere_fitting;

       /** Result-matrix(3x1) of the SVD-calculation of the offset. */
       TNT::Matrix<double> result_svd;

       /** Enumeration for different states of the programflow.
        */
       enum Status { /** Waiting for some input from tracking-source. */
                     WAIT_4_EVENT,
                     /** Waiting for keyhit to store the 1st point. */ 
                     WAIT_4_1st_POINT,
                     /** Storing the 1st point to calculate the minimal
                                           distance between the samples. */
                     RECORD_1st_POINT, 
                     /** Waiting for the keyhit to store the 2nd point. */
                     WAIT_4_2nd_POINT, 
                     /** Storing the 2nd point to calculate minimal distance.*/
                     RECORD_2nd_POINT,  
                     /** Storing the tracked points. */
                     COLLECT_SAMPLES,  
                     /** Storing compleded -> starting calculation.*/
                     ALL_FINISHED      
               } status;  

       /** just 4 testing and debugging */
       bool print_point2console; 
	           
        // Methods
    public:

       /** Constructor method
        *  @param number_of_samples_ number of samples used for the calculation
        *       If number_of_samples_ = 0 then STANDARD_NUMBER_OF_SAMPLES is used. 
        *  @param filename_ name of the ART-Configuration file
        *       If a filename is given, the file will be modified after the calculation.
        *  @param show_scatter_ if TRUE a scatterplot will be showen after the calculation
        *  @param save_result_ if TRUE a file (result.txt) with the result and the used
        *      samples will be written.  */
       StylusCalibration(int number_of_samples_, std::string filename_, bool show_scatter_, bool save_result_);

    private:
       /** Copyconstructor method*/
       StylusCalibration(StylusCalibration &src){}

    public:
       /** Destructor method*/
       virtual ~StylusCalibration(){}

       /** 'main' routine.
        *  Depending on the program-status it starts the
        *  necessary methods.
        */
       void handleEvent( Event & event);

       /** Checks if some keyboard input occured.
        */
       void checkKeyboard();

       /** Stores the actual translation and orientation value
        *  to translation_samples and orientation_samples.
        */
       void updateSampleVector( Event & event );

       /** Calculating the stylus offset by using the stored Samples.
        */
       void calcStylus();

       /** Checks if the distance between two points, specified by the two 
        *  vectors vector_a and vector_b, is greater than the distance
        *  stored in minimal_distance.
        *  @param vector_a matrix-representation(3x1) of the first point
        *  @param vector_b matrix-representation(3x1) of the second point
        *  @return TRUE if the distance betwenn vector_a and vector_b is
        *            greater than minimal_distance, FALSE else. */
       bool checkDistance(TNT::Matrix<double> const & vector_a, TNT::Matrix<double> const & vector_b);

       /** Calculates the minimal distance between two points for their
        * use in the calculation.
        * (it is just an approximation, but ist works well)  */
       float calculateMinimumDistance();

       /** Modifies the ART-Configuration file.
        * It changes the points in the configuration file so that
        * the zero-point is set to the calculated top of the stylus. */
       void modifyFile();

       /** Helping function for modifyFile() which reads the stored
        *  points form the file. 
        *  @param number_of_points reference to an int to store the number of points 
        *  @param points reference to a \<3x1 matrix\>vector to store the points
        *  @param text reference to a \<char\>vector to store the remaining text from the file
        *  @return TRUE if reading was succesfull, FALSE else. */
       bool readFromFile(int & number_of_points, std::vector<TNT::Matrix<double>> & points,
                     std::vector<char> & text);

       /** Helping function for modifyFile() which writes the new
        *  calculated points back to the file.
        *  @param number_of_points reference to an int were the number of points is stored
        *  @param points reference to a \<3x1 matrix\>vector were the original points to write are stored
        *  @param modified_points reference to a \<3x1 matrix\>vector were the new calculated points to write are stored
        *  @param text reference to a \<char\>vector were the remaining text from the file is stored
        *  @return TRUE if writing was succesfull, FALSE else. */
       bool writeToFile(int & number_of_points, std::vector<TNT::Matrix<double>> & points,
                     std::vector<TNT::Matrix<double>> & modified_points, std::vector<char> & text);

       /** Generates a .iv file which shows a scatterplot of the
        *  samples used for calculation and stores it to "./plot/scatter.iv". */
       void writeScatterPlot();

       /** Starts a new instance of studierstube and shows the
        *  scatterplot stored in "./plot/scatter.iv".  */
       void showScatter();

       /** Generates result.txt were the result and the coordinates of
        * the used samples are stored as plaintext. */
       void writeResult();

       /** Calculates the length of a vector represented by a 3x1 matrix. */
       float getLength(TNT::Matrix<double> const & vector);

       /** Calculates the center of the hemisphere specified by the given samples.
        * <p> This method based on a furter work of Rainer Splechtna.
        * <em>[Rainer Splechtna, Comprehensive Calibration Procedures for Augmented Realitiy,
        * TU Wien, 2000]</em></p>
        */
       TNT::Matrix<double> SVDStylusOptimization();

       /** helping function for SVDStylusOptimization() @see SVDStylusOptimization() */
       TNT::Array2D<double> generateMatrix(TNT::Matrix<double> in);

       /** helping function for SVDStylusOptimization() @see SVDStylusOptimization() */
       TNT::Matrix<double> calculateMatrix(TNT::Matrix<double> const & orientation);

    };

} // namespace ot

#endif //_STYLUS_CALIBRATION_H