/*! @page virtual_cockpit Virtual Cockpit -  A Graphical User Interface Extension
 *
 * \n
 * @image html vc.png "Virtual Cockpit in action."
 * \n
 *
 * \section intro_sec Introduction
 *
 * The <em> Virtual Cockpit </em>  is an approach for a hierarchical graphical
 * user interface in an Augmented Reality environment, which allows fast and
 * intuitive navigation even when dealing with complex layouts.
 * This project enhances the "Studierstube" library by
 * providing a head-fixed 3-dimensional menu hierarchy as
 * an alternative to the personal interaction panel.
 * Although providing a natural haptic feedback we have experienced different
 * limitations in terms of usability and flexibility by using this personal
 * interaction panel for application control and navigation.
 * This project provides a different user interface
 * providing a complete 3-dimensional experience. Moreover, a methodology
 * for dealing with complex interaction layouts by introducing a smart tree structure
 * is also given.
 * \n
 * With the <em>Virtual Cockpit </em> we provide a container like module structure
 * as follows:
 *
 * \li \c MainPanel (class <tt>SoVCMainPanelKit</tt>) contains and manages number of \c SubPanels (class <tt>SoVCSubPanelKit</tt>)
 * \li \c SubPanel (class <tt>SoVCSubPanelKit</tt>) contains and manages number of \c Widgets (base class <tt>SoVCWidgetKit</tt>)
 * \li \c Widget (base class <tt>SoVCWidgetKit</tt>) contains zero to unlimited further \c Widgets (base class <tt>SoVCWidgetKit</tt>)
 *
 * \n
 * \section install_sec Installation
 *
 * The following steps should help to set up the <em> Virtual Cockpit </em> for a
 * user defined application. Therefore changes will be necessary to this file:
 *
 * \li \c /bin/workspace/defaultMPKit.iv
 * 
 * Optionally you may have to modify this files to customize the <em> Virtual Cockpit's </em>
 * overall settings:
 *
 * \li \c /bin/workspace/yourUserKit.iv
 * \li \c /bin/workspace/graphix/skinVC.iv
 * 
 * \subsection specify_layout_ssec Specifying Your Widget's Layout
 *
 * The layout of the Widgets for your application is simply done by specifying your
 * Widget's through a hierarchy like manner in an OpenInventor ascii-file. Whether
 * you modify the <tt>defaultMPKit.iv</tt> file or maintain several layouts in a
 * number of files is your choice.
 * \n
 * The different Widget behaviors available to you so far are:
 * \li <b>slider</b> / <b>dial</b> like
 * \li <b>push-button</b> like
 * \li <b>toggle-button</b> like
 * \li special <b>calibration</b> behavior
 *
 * This is an example configuration like you find it in <tt>defaultMPKit.iv</tt>:
 *
 * \htmlonly
<table class="mdTable" cellpadding="2" cellspacing="0" width="100%">
  <tbody><tr>
    <td class="mdRow">
      <table border="0" cellpadding="0" cellspacing="0">
        <tbody><tr>
          <td class="md" nowrap="nowrap" valign="top">
          <pre> 
      #Inventor V2.1 ascii
        
       SoVCMainPanelKit 
       {
           subPanels SoSwitch
           {           
                SoVCSubPanelKit 
                {
                    title "SYSTEM"                    
                    widgets Switch 
                    {                
                         DEF WIDGET_0 SoVCCalibrationWidgetKit {}
                    }
                }
                SoVCSubPanelKit 
                {
                    title "COLORS"                     
                    widgets Switch 
                    {              
                         DEF WIDGET_1 SoVCPushWidgetKit 
                         { 
                             widgetName "RED"
                             scaleContent FALSE
                             widgetContent Switch 
                             {                         
                                 Separator 
                                 { 
                                     Material { diffuseColor 1 0 0 }
                                     Transform { rotation 1 0 0 -1
                                                 scaleFactor 0.2 0.2 0.2 
                                               }
                                     Cube { width 1 height 1 depth 1 }
                                 }
                             }
                         }
                        DEF WIDGET_2 SoVCDialWidgetKit
                        {
                             widgetName "EXTRA"
                             hideWhenLeaf FALSE
                        }
                     }
                 }
             }
         }    
           </pre>
           </td>          
          <td class="md">&nbsp;</td>
          <td class="md" colspan="2"></td>
        </tr>
      </tbody></table>
    </td>
  </tr>
</tbody></table>
\endhtmlonly 
 *
 * The definition of the <tt>SoVCMainPanelKit</tt> node is your starting point. Since
 * it contains the SubPanels you define here your <tt>SoVCSubPanelKit</tt>'s. Each 
 * SubPanel can have a \em SoVCSubPanelKit::title (used by the StatusBar to display your position in
 * the menu hierarchy) and has to have it's \em SoVCSubPanelKit::widgets .
 * 
 * Subsequent to defining you SubPanel you go on with your <tt>SoVCWidgetKit</tt>'s. 
 * Each Widget should have a \em SoVCWidgetKit::widgetName (used by the StatusBar to display your position 
 * in the menu hierarchy). In addition you can specify whether the Widget's content should be
 * scaled to fit the sphere's whole inner space or not with the \em SoVCWidgetKit::scaleContent field.
 * The content itself is specified through \em SoVCWidgetKit::widgetContent and can be any possible
 * OpenInventor scenegraph.
 * And if you want your Widget not to be hidden when it becomes activated at a leaf-position in the hierarchy,
 * then specify \em SoVCWidgetKit::hideWhenLeaf as <tt>TRUE</tt>.
 *
 * Finally in this section follows an inside view on interaction of each specific Widget
 * with your application:
 * 
 * \subsubsection app_iaction_push_sssec SoVCPushWidgetKit
 *
 * In you application code you simply override the method
 * <em>SoContextKit::checkMainPanelMasterMode(SoNode *mainPanelContents, SbBool masterMode)</em>
 * and inside you are going to set up callbacks for press and release events which
 * could happen to your Widget. First you have to get the Widget you previously defined
 * by using it's \em "DEF" name:
 *
 * <tt>w = (SoVCWidgetKit*)findNode(mainPanelContents, wName);</tt>
 *
 * Then you add the callback of your choice to the Widget:
 *
 * <tt>w->addPressCallback(colorWidgetCB, this);</tt>
 *\n
 * or
 *\n
 * <tt>w->addReleaseCallback(colorWidgetCB, this);</tt>
 *
 * \subsubsection app_iaction_toggle_sssec SoVCToggleWidgetKit
 *
 * In you application code you simply override the method
 * <em>SoContextKit::checkMainPanelMasterMode(SoNode *mainPanelContents, SbBool masterMode)</em>
 * and inside you are going to set up callbacks for press and release events which
 * could happen to your Widget. First you have to get the Widget you previously defined
 * by using it's \em "DEF" name:
 *
 * <tt>w = (SoVCWidgetKit*)findNode(mainPanelContents, wName);</tt>
 *
 * Then you add the callback of your choice to the Widget:
 *
 * <tt>w->addPressCallback(colorWidgetCB, this);</tt>
 *\n
 * or
 *\n
 * <tt>w->addReleaseCallback(colorWidgetCB, this);</tt>
 *
 * In the callback function you are able to read the Widget's current toggle state
 * with <em>SoVCToggleWidgetKit::getToggleState()</em>.
 *
 * \subsubsection app_iaction_dial_sssec SoVCDialWidgetKit
 *
 * In you application code you simply override the method
 * <em>SoContextKit::checkMainPanelMasterMode(SoNode *mainPanelContents, SbBool masterMode)</em>
 * and inside you are going to set up callbacks for press and release events which
 * could happen to your Widget. First you have to get the Widget you previously defined
 * by using it's \em "DEF" name:
 *
 * <tt>w = (SoVCWidgetKit*)findNode(mainPanelContents, wName);</tt>
 *
 * Then you add the callback of your choice to the Widget:
 *
 * <tt>w->addPressCallback(colorWidgetCB, this);</tt>
 *\n
 * or
 *\n
 * <tt>w->addReleaseCallback(colorWidgetCB, this);</tt>
 *\n
 * or
 *\n
 ** <tt>w->addValueCallback(colorWidgetCB, this);</tt>
 *
 * In the callback function you are able to read the Widget's current dial value
 * with <em>SoVCDialWidgetKit::getValue()</em>, whereas the Widget's current 
 * dial angle can be read with <em>SoVCDialWidgetKit::getAngle()</em>.
 * 
 * \subsubsection app_iaction_calib_sssec SoVCCalibrationWidgetKit
 *
 * In you application code you simply override the method
 * <tt>checkMainPanelMasterMode(SoNode *mainPanelContents, SbBool masterMode)</tt>
 * and inside you are going to set up callbacks for press and release events which
 * could happen to your Widget. First you have to get the Widget you previously defined
 * by using it's \em "DEF" name:
 *
 * <tt>w = (SoVCWidgetKit*)findNode(mainPanelContents, wName);</tt>
 *
 * Then you add the callback of your choice to the Widget:
 *
 * <tt>w->addPressCallback(colorWidgetCB, this)</tt>
 *\n
 * or
 *\n
 * <tt>w->addReleaseCallback(colorWidgetCB, this)</tt>
 *
 * This Widget is an extension of the <tt>SoVCPushWidgetKit</tt> and is capable of 
 * calibrating the <em>Virtual Cockpit</em> to the optimal position and size, depending
 * on the users prefered arm bend. You simply press the Widget and wait with the release
 * until you found the most comfortable position - then release it and voila.
 *
 * \subsection mod_startup_params_ssec Modifying Startup Parameters
 *
 * To activate the <em>Virtual Cockpit</em> for your application you have to add the
 * following lines to the file containing your <tt>SoUserKit</tt> which may allready 
 * have the PEN, PIP configuration inside.
 *
 * A sample file could look like this:
 * 
 * \htmlonly
<table class="mdTable" cellpadding="2" cellspacing="0" width="100%">
  <tbody><tr>
    <td class="mdRow">
      <table border="0" cellpadding="0" cellspacing="0">
        <tbody><tr>
          <td class="md" nowrap="nowrap" valign="top">
          <pre>
 // ... PEN setup
 // ... PIP setup
 // ... display setup
  
 vc SoVCKit
   {
      station 2      
      vcParts (VC_WINDOWBAR)
      pinned TRUE
      offset Transform
      {
        translation 0 0 0
        rotation 0 0 0 0
        #scaleFactor 1.25 1.25 1.25
      }
   }
          </pre>
           </td>          
          <td class="md">&nbsp;</td>
          <td class="md" colspan="2"></td>
        </tr>
      </tbody></table>
    </td>
  </tr>
</tbody></table>
\endhtmlonly 
 *
 * Where the field <em>SoVCKit::vcParts</em> specifys the <em>Virtual Cockpit's</em> visible
 * components at startup using values from <em>SoVCKit::VCPartMask</em>.
 * In addition the field <em>SoVCKit::pinned</em> determines whether the position is initially fixed or not.
 *
 * \subsection custom_layout_ssec Customizing Layout Behaviour
 *
 * Although not recommended it's possible to customize general visualization of the <em>Virtual Cockpit</em>
 * by modifying things like Widget size, position of Bars and so on. For further details we refer to the file
 * <tt>skinVC.iv</tt>.
 * \n
 * The one case when you should look into this file is when setting up <em>Virtual Cockpit's</em> basic <b>calibration 
 * information</b> regarding you VR/AR-setup. The specific parameters are located at the very end of the file.
 *
 */
