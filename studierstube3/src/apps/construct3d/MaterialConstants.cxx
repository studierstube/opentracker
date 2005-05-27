// Material Constants
// Copyright by Menega Sabidussi & Hannes Kaufmann, 2003


#include <Inventor/nodes/SoMaterial.h>
#include "MaterialConstants.h"
#include <Inventor/nodes/SoTexture2.h>

#ifdef WIN32
#pragma warning(disable:4305)
#endif

#define blue 0
#define orange 1
#define green 2
#define red 3

// assign colors as defined in userXColor to users with user ID
int
MaterialConstants::assignUserColors(int userID)
{
	if ((userID % 4) == 0)
		return user1Color;
	else if ((userID % 4) == 1)
		return user2Color;
	else if ((userID % 4) == 2)
		return user3Color;
	else if ((userID % 4) == 3)
		return user4Color;
	return user1Color;
}

// Point Materials
void
MaterialConstants::definePointMaterials(SoMaterial& Pw, SoMaterial& Ps, SoMaterial& Pd, SoMaterial& Ph, SoMaterial& Pl, int userID)
{
	setHighlightGridColor(Ph, userID);
	int userColor = assignUserColors(userID);
	if (userColor == blue) // blue scheme
	{
		//Working color
        Pw.ambientColor.setValue(0, 0, 0);
		Pw.diffuseColor.setValue(0, 0.030372, 0.09);
		Pw.specularColor.setValue(0.144162, 0.279308, 0.55);
		Pw.emissiveColor.setValue(0.735802, 0.721606, 0.75);
		Pw.shininess.setValue(0.12);

        // highlight color  
		// set in setHighlightGridColor()

		//draggable  
		Pd.ambientColor.setValue(0,0,0);
		Pd.diffuseColor.setValue(0.141839, 0.264497, 0.39);
		Pd.specularColor.setValue(1, 0.0123162, 0.807356);
		Pd.emissiveColor.setValue(0.0465862, 0.0816971, 0.14);
		Pd.shininess.setValue(0.09);

		// selected
		Ps.ambientColor.setValue(0.00881993, 0.0277882, 0.0476192);
		Ps.diffuseColor.setValue(0.0740872, 0.23342, 0.4);
		Ps.specularColor.setValue(0.32, 0.4, 0.4);
		Ps.emissiveColor.setValue(0.07, 0.13, 0.22);
		Ps.shininess.setValue(0.54);

		// layer deactivated
		Pl.ambientColor.setValue(0, 0.00091116, 0.0027);
		Pl.diffuseColor.setValue(0, 0.030372, 0.09);
		Pl.specularColor.setValue(0.0786338, 0.15235, 0.3);
		Pl.emissiveColor.setValue(0.378073, 0.351227, 0.38);
		Pl.shininess.setValue(0.12);
	} else
    if (userColor == orange) // orange scheme
	{
		//Working color
		Pw.ambientColor.setValue(0.0021, 0.00116555, 0.000211501);
		Pw.diffuseColor.setValue(0.07, 0.0388516, 0.00705003);
		Pw.specularColor.setValue(0.39, 0.213492, 0.0806066);
		Pw.emissiveColor.setValue(0.62, 0.585212, 0.573476);
		Pw.shininess.setValue(0.12);

  		// highlight color
		// using orange nearest line color
		Ph.diffuseColor.setValue(0.9, 0.510321, 0.0503342);

		//draggable  
		Pd.ambientColor.setValue(0, 0, 0);
		Pd.diffuseColor.setValue(0.0121634, 0.2, 0.0144907);
		Pd.specularColor.setValue(0.0677456, 1, 0.0535242);
		Pd.emissiveColor.setValue(0.69, 0, 0.165829);
		Pd.shininess.setValue(0.07);

		// selected
		Ps.ambientColor.setValue(0.2236, 0.133399, 0.0572109);
		Ps.diffuseColor.setValue(0.43, 0.256536, 0.110021);
		Ps.specularColor.setValue(0.29, 0.31, 0.05);
		Ps.emissiveColor.setValue(0.21, 0.13, 0.08);
		Ps.shininess.setValue(0.36);

		// layer deactivated
		Pl.ambientColor.setValue(0.0036, 0.00166893, 0.000101564);
		Pl.diffuseColor.setValue(0.12, 0.0556309, 0.00338546);
		Pl.specularColor.setValue(0.3, 0.174264, 0.032293);
		Pl.emissiveColor.setValue(0.307066, 0.31, 0.304132);
		Pl.shininess.setValue(0.12);
	} else
	if (userColor == green) // green scheme
	{
		//Working color
		Pw.ambientColor.setValue(0.000632874, 0.0027, 0.00157559);
		Pw.diffuseColor.setValue(0.0210958, 0.09, 0.0525196);
		Pw.specularColor.setValue(0.21044, 0.55, 0.393831);
		Pw.emissiveColor.setValue(0.539462, 0.577285, 0.6);
		Pw.shininess.setValue(0.12);

  		// highlight color
		Ph.diffuseColor.setValue(0.674137, 0.78, 0.440404);

		//draggable  
		Pd.ambientColor.setValue(0, 0, 0);
		Pd.diffuseColor.setValue(0.0162211, 0.3, 0.175065);
		Pd.specularColor.setValue(1, 0.138756, 0.0251787);
		Pd.emissiveColor.setValue(0.14, 0.0777032, 0.0141001);
		Pd.shininess.setValue(0.09);

		// selected
		Ps.ambientColor.setValue(0.0959999, 0.16, 0.0746666);
		Ps.diffuseColor.setValue(0.18, 0.3, 0.14);
		Ps.specularColor.setValue(0.363004, 0.425516, 0.47);
		Ps.emissiveColor.setValue(0.14, 0.08, 0.07);
		Ps.shininess.setValue(0.45);

		// layer deactivated
		Pl.ambientColor.setValue(0.00146968, 0.0021, 0.00194099);
		Pl.diffuseColor.setValue(0.0489892, 0.07, 0.0646997);
		Pl.specularColor.setValue(0.0512669, 0.24, 0.153681);
		Pl.emissiveColor.setValue(0.27257, 0.3, 0.294321);
		Pl.shininess.setValue(0.12);
	} else
	if (userColor == red) // red scheme
	{
		//Working color
		Pw.ambientColor.setValue(0.0027, 0, 0.000916305);
		Pw.diffuseColor.setValue(0.09, 0, 0.0305435);
		Pw.specularColor.setValue(0.467476, 0.439706, 0.53);
		Pw.emissiveColor.setValue(0.598478, 0.577285, 0.6);
		Pw.shininess.setValue(0.12);

  		// highlight color
		Ph.ambientColor.setValue(0, 0, 0);
		Ph.diffuseColor.setValue(1, 0.351688, 0.730279);
		Ph.specularColor.setValue(0, 0, 0);
		Ph.emissiveColor.setValue(0, 0, 0);
		Ph.shininess.setValue(0);
		Ph.transparency.setValue(0);

		//draggable  
		Pd.ambientColor.setValue(0, 0, 0);
		Pd.diffuseColor.setValue(0, 0, 0);
		Pd.specularColor.setValue(0.81, 0.679012, 0);
		Pd.emissiveColor.setValue(0.511877, 0, 0.54);
		Pd.shininess.setValue(0.04);

		// selected
		Ps.ambientColor.setValue(0.037647, 0.00849367, 0.0198959);
		Ps.diffuseColor.setValue(0.16, 0.0360981, 0.0845576);
		Ps.specularColor.setValue(0.84, 0.533777, 0.78819);
		Ps.emissiveColor.setValue(0.3, 0.159456, 0.165134);
		Ps.shininess.setValue(0.54);

		// layer deactivated
		Pl.ambientColor.setValue(0.0024, 0, 0.000814494);
		Pl.diffuseColor.setValue(0.08, 0, 0.0271498);
		Pl.specularColor.setValue(0.313208, 0.198055, 0.39);
		Pl.emissiveColor.setValue(0.319188, 0.307885, 0.32);
		Pl.shininess.setValue(0.11);
	}
}

void
MaterialConstants::defineLineMaterials(SoMaterial& Pw, SoMaterial& Ps, SoMaterial& Pd, SoMaterial& Ph, SoMaterial& Pl, int userID)
{
	int userColor = assignUserColors(userID);
	if (userColor == blue) // blue scheme
	{
		//Working color
        Pw.ambientColor.setValue(0., 0., 0.);
        Pw.diffuseColor.setValue(0.718552, 0.738251, 0.87);

        // highlight color  
        Ph.ambientColor.setValue(0., 0., 0.);
		Ph.diffuseColor.setValue(0.433694, 0.57329, 0.78);

 		//draggable - line not draggable

		// selected
        Ps.ambientColor.setValue(0, 0, 0);
		Ps.diffuseColor.setValue(0, 0.315, 0.7);

		// layer deactivated
        Pl.ambientColor.setValue(0, 0, 0);
        Pl.diffuseColor.setValue(0.350129, 0.355997, 0.4);
	} else
	if (userColor == orange) // orange scheme
	{
		// working color
        Pw.ambientColor.setValue(0., 0., 0.);
        Pw.diffuseColor.setValue(0.85, 0.639983, 0.446902);
		Pw.emissiveColor.setValue(0, 0, 0);
		Pw.shininess.setValue(0);

        // highlight color  
        Ph.ambientColor.setValue(0., 0., 0.);
		Ph.diffuseColor.setValue(0.9, 0.510321, 0.0503342);

		// selected
        Ps.ambientColor.setValue(0, 0, 0);
		Ps.diffuseColor.setValue(0.63, 0.364357, 0.0900688);
		Ps.emissiveColor.setValue(0, 0, 0);
		Ps.shininess.setValue(0);

		// layer deactivated
        Pl.ambientColor.setValue(0, 0, 0);
        Pl.diffuseColor.setValue(0.39, 0.278874, 0.219814);
		Pl.emissiveColor.setValue(0, 0, 0);
		Pl.shininess.setValue(0);
	} else
	if (userColor == green) // green scheme
	{
		// working color
        Pw.ambientColor.setValue(0., 0., 0.);
        Pw.diffuseColor.setValue(0.659251, 0.8, 0.754569);
		Pw.emissiveColor.setValue(0, 0, 0);
		Pw.shininess.setValue(0);

        // highlight color  
        Ph.ambientColor.setValue(0., 0., 0.);
		Ph.diffuseColor.setValue(0.674137, 0.78, 0.440404);

		// selected
		Ps.ambientColor.setValue(0, 0, 0);
		Ps.diffuseColor.setValue(0.41921, 0.5, 0.272846);
		Ps.emissiveColor.setValue(0, 0, 0);
		Ps.shininess.setValue(0);

		// layer deactivated
        Pl.ambientColor.setValue(0, 0, 0);
        Pl.diffuseColor.setValue(0.328666, 0.36, 0.332742);
		Pl.emissiveColor.setValue(0, 0, 0);
		Pl.shininess.setValue(0);
	} else
	if (userColor == red) // red scheme
	{
		// working color
        Pw.ambientColor.setValue(0., 0., 0.);
        Pw.diffuseColor.setValue(0.8, 0.578534, 0.654253);
		Pw.emissiveColor.setValue(0, 0, 0);
		Pw.shininess.setValue(0);

        // highlight color  
        Ph.ambientColor.setValue(0., 0., 0.);
		Ph.diffuseColor.setValue(1, 0.351688, 0.730279);

		// selected
		Ps.ambientColor.setValue(0, 0, 0);
		Ps.diffuseColor.setValue(0.63, 0.22242, 0.293974);
		Ps.emissiveColor.setValue(0, 0, 0);
		Ps.shininess.setValue(0);

		// layer deactivated
        Pl.ambientColor.setValue(0, 0, 0);
        Pl.diffuseColor.setValue(0.39, 0.227921, 0.309128);
		Pl.emissiveColor.setValue(0, 0, 0);
		Pl.shininess.setValue(0);
	}
}
/*
static void
MaterialConstants::defineIntersectionCurveMaterials(SoMaterial& Pw, SoMaterial& Ps, SoMaterial& Pd, SoMaterial& Ph, SoMaterial& Pl, int userID)
{
	if ((userID % 4) == 0) // blue scheme
	{
		//Working color
		Pw.diffuseColor.setValue(1.0, 1.0, 1.0);

		// highlight
		Ph.diffuseColor.setValue(1.0, 1.0, 1.0);

		//draggable
		Pd.diffuseColor.setValue(1.0, 1.0, 1.0);

		//selected
		Ps.ambientColor.setValue(0.0941176, 0, 0.0753343);
		Ps.diffuseColor.setValue(0.4, 0, 0.320171);
		Ps.specularColor.setValue(0.84, 0.74, 0.07);
		Ps.emissiveColor.setValue(0.16, 0, 0.116765);
		Ps.shininess.setValue(0.54);

   		// layer deactivated
		Pl.ambientColor.setValue(0, 0.00091116, 0.0027);
		Pl.diffuseColor.setValue(0, 0.030372, 0.09);
	}
}
*/
// sets the plane materials in case no texture would be used or
// if MODULATE mode would be used which combines material and texture
void
MaterialConstants::definePlaneMaterials(SoMaterial& Pw, SoMaterial& Ps, SoMaterial& Pd, SoMaterial& Ph, SoMaterial& Pl, int userID)
{
	setHighlightGridColor(Ph, userID);
	int userColor = assignUserColors(userID);
	if (userColor == blue) // blue scheme
	{
		//Working color
        Pw.ambientColor.setValue(0, 0, 0);
        Pw.diffuseColor.setValue(0, 0, 0);
		Pw.emissiveColor.setValue(0.880709, 0.895352, 1);
		Pw.shininess.setValue(1);
        Pw.transparency.setValue(0.4);

		// highlight grid color
		// set in setHighlightGridColor

 		//draggable - plane not draggable
		Pd.emissiveColor.setValue(0, 0, 0);

		// selected
        Ps.ambientColor.setValue(0, 0, 0);
        Ps.diffuseColor.setValue(0, 0, 0);
        Ps.emissiveColor.setValue(0.468299, 0.526762, 1);
		Ps.shininess.setValue(1);
        Ps.transparency.setValue(0.5);

		// layer deactivated
        Pl.ambientColor.setValue(0, 0, 0);
        Pl.diffuseColor.setValue(0, 0, 0);
		Pl.emissiveColor.setValue(0.674165, 0.72128, 0.85);
		Pl.shininess.setValue(1);
        Pl.transparency.setValue(0.5);

	} else
    if (userColor == orange) // orange scheme
	{
		//Working color
        Pw.ambientColor.setValue(0, 0, 0);
        Pw.diffuseColor.setValue(0, 0, 0);
		Pw.specularColor.setValue(0,0,0);
		Pw.emissiveColor.setValue(1, 0.860067, 0.74649);
		Pw.shininess.setValue(1);
        Pw.transparency.setValue(0.4);

		// highlight grid color  

		// selected
        Ps.emissiveColor.setValue(1, 0.516485, 0.2704);
		Ps.shininess.setValue(1);
        Ps.transparency.setValue(0.5);

		// layer deactivated
        Pl.ambientColor.setValue(0, 0, 0);
        Pl.diffuseColor.setValue(0, 0, 0);
		Pl.specularColor.setValue(0, 0, 0);
		Pl.emissiveColor.setValue(0.85, 0.728902, 0.664541);
		Pl.shininess.setValue(1);
        Pl.transparency.setValue(0.5);
	} else
	if (userColor == green) // green scheme
	{
		//Working color
        Pw.ambientColor.setValue(0, 0, 0);
        Pw.diffuseColor.setValue(0, 0, 0);
		Pw.specularColor.setValue(0,0,0);
		Pw.emissiveColor.setValue(0.798208, 1, 0.924281);
		Pw.shininess.setValue(1);
        Pw.transparency.setValue(0.4);

		// selected
        Ps.emissiveColor.setValue(0.648627, 1, 0.526762);
		Ps.shininess.setValue(1);
        Ps.transparency.setValue(0.5);

		// layer deactivated
        Pl.ambientColor.setValue(0, 0, 0);
        Pl.diffuseColor.setValue(0, 0, 0);
		Pl.specularColor.setValue(0, 0, 0);
		Pl.emissiveColor.setValue(0.684365, 0.85, 0.76955);
		Pl.shininess.setValue(1);
        Pl.transparency.setValue(0.5);
	} else
	if (userColor == red) // red scheme
	{
		//Working color
        Pw.ambientColor.setValue(0, 0, 0);
        Pw.diffuseColor.setValue(0, 0, 0);
		Pw.specularColor.setValue(0,0,0);
		Pw.emissiveColor.setValue(1, 0.8146, 0.890318);
		Pw.shininess.setValue(1);
        Pw.transparency.setValue(0.4);

		// selected
        Ps.emissiveColor.setValue(1, 0.055927, 0.567025);
		Ps.shininess.setValue(1);
        Ps.transparency.setValue(0.5);

		// layer deactivated
        Pl.ambientColor.setValue(0, 0, 0);
        Pl.diffuseColor.setValue(0, 0, 0);
		Pl.specularColor.setValue(0, 0, 0);
		Pl.emissiveColor.setValue(0.85, 0.663963, 0.840955);
		Pl.shininess.setValue(1);
        Pl.transparency.setValue(0.5);
	}
}

void
MaterialConstants::defineBoolMaterials(SoMaterial& Pw, SoMaterial& Ps, SoMaterial& Pd, SoMaterial& Ph, SoMaterial& Pl, int userID)
{
	setHighlightGridColor(Ph, userID);
	int userColor = assignUserColors(userID);
	if (userColor == blue) // blue scheme
	{
		//Working color
		Pw.ambientColor.setValue(0, 0.00091116, 0.0027);
		Pw.diffuseColor.setValue(0, 0.030372, 0.09);
        Pw.specularColor.setValue(0.157268, 0.3047, 0.6);
		Pw.emissiveColor.setValue(0.559135, 0.577285, 0.6);
		Pw.shininess.setValue(0.12);

		// highlight grid color
		// set in setHighlightGridColor

        // selected
		Ps.ambientColor.setValue(0, 0.0375001, 0.0833336);
		Ps.diffuseColor.setValue(0, 0.315, 0.7);
		Ps.specularColor.setValue(0, 0.41372, 0.43);
		Ps.emissiveColor.setValue(0.00120639, 0.00864229, 0.02);
		Ps.shininess.setValue(0.25);

		// layer deactivated
        Pl.ambientColor.setValue(0, 0.00091116, 0.0027);
        Pl.diffuseColor.setValue(0, 0.030372, 0.09);
        Pl.specularColor.setValue(0.125494, 0.200496, 0.28);
		Pl.emissiveColor.setValue(0.363437, 0.375235, 0.39);
		Pl.shininess.setValue(0.29);
    } else 
	if (userColor == orange) // orange scheme
	{
		//Working color
		Pw.ambientColor.setValue(0.0021, 0.00116555, 0.000211501);
		Pw.diffuseColor.setValue(0.07, 0.0388516, 0.00705003);
        Pw.specularColor.setValue(0.39, 0.213492, 0.0806066);
		Pw.emissiveColor.setValue(0.6, 0.567856, 0.533783);
		Pw.shininess.setValue(0.12);

        // selected
		Ps.ambientColor.setValue(0.0464287, 0.0239797, 0.0125543);
		Ps.diffuseColor.setValue(0.39, 0.201429, 0.105456);
		Ps.specularColor.setValue(0.347656, 0.46, 0.190065);
		Ps.emissiveColor.setValue(0.21, 0.152683, 0.108956);
		Ps.shininess.setValue(0.25);
		
		// layer deactivated
        Pl.ambientColor.setValue(0.003, 0.00149674, 7.70286e-005);
        Pl.diffuseColor.setValue(0.1, 0.0498914, 0.00256762);
        Pl.specularColor.setValue(0.33, 0.184383, 0.0969288);
		Pl.emissiveColor.setValue(0.336782, 0.333564, 0.34);
		Pl.shininess.setValue(0.29);
	} else
	if (userColor == green) // green scheme
	{
		//Working color
		Pw.ambientColor.setValue(0.000632874, 0.0027, 0.00157559);
		Pw.diffuseColor.setValue(0.0210958, 0.09, 0.0525196);
        Pw.specularColor.setValue(0.229571, 0.6, 0.429634);
		Pw.emissiveColor.setValue(0.539462, 0.577285, 0.6);
		Pw.shininess.setValue(0.12);

        // selected
		Ps.ambientColor.setValue(0.0207328, 0.0357144, 0.0174609);
		Ps.diffuseColor.setValue(0.174155, 0.3, 0.146671);
		Ps.specularColor.setValue(0.272929, 0.461103, 0.47);
		Ps.emissiveColor.setValue(0.14, 0.08, 0.07);
		Ps.shininess.setValue(0.25);

		// layer deactivated
        Pl.ambientColor.setValue(0.00188958, 0.0027, 0.00249556);
        Pl.diffuseColor.setValue(0.0629861, 0.09, 0.0831854);
        Pl.specularColor.setValue(0.162011, 0.33, 0.123857);
		Pl.emissiveColor.setValue(0.305237, 0.323753, 0.33);
		Pl.shininess.setValue(0.29);
	} else
	if (userColor == red) // red scheme
	{
		//Working color
		Pw.ambientColor.setValue(0.0027, 0, 0.000916305);
		Pw.diffuseColor.setValue(0.09, 0, 0.0305435);
        Pw.specularColor.setValue(0.558174, 0.455692, 0.62);
		Pw.emissiveColor.setValue(0.518681, 0.500314, 0.52);
		Pw.shininess.setValue(0.11);

        // selected
		Ps.ambientColor.setValue(0.032143, 0.00899571, 0.0162971);
		Ps.diffuseColor.setValue(0.27, 0.0755637, 0.136895);
		Ps.specularColor.setValue(0.84, 0.533777, 0.78819);
		Ps.emissiveColor.setValue(0.26, 0.155421, 0.125891);
		Ps.shininess.setValue(0.25);

		// layer deactivated
        Pl.ambientColor.setValue(0.0024, 0, 0.000814494);
        Pl.diffuseColor.setValue(0.08, 0, 0.0271498);
        Pl.specularColor.setValue(0.313208, 0.198055, 0.39);
		Pl.emissiveColor.setValue(0.379036, 0.365613, 0.38);
		Pl.shininess.setValue(0.29);
	}
}

void
MaterialConstants::defineTransparentMaterials(SoMaterial& Pw, SoMaterial& Ps, SoMaterial& Pd, SoMaterial& Ph, SoMaterial& Pl, int userID)
{
	int userColor = assignUserColors(userID);
	// setHighlightGridColor(Ph, userID);
	if (userColor == blue) // blue scheme
	{
        //Working color
		Pw.ambientColor.setValue(0, 0, 0);
		Pw.diffuseColor.setValue(0.581197, 0.62141, 1);
		Pw.specularColor.setValue(0.878135, 0.526762, 1);
		Pw.emissiveColor.setValue(0.418041, 0.481984, 0.8);
		Pw.shininess.setValue(0.05);
		Pw.transparency.setValue(0.5);

		// highlight grid color
		// set in setHighlightGridColor

		//draggable  
		Pd.ambientColor.setValue(0.0333739, 0.0622346, 0.0917647);
		Pd.diffuseColor.setValue(0.141839, 0.264497, 0.39);
		Pd.specularColor.setValue(0.84, 0.073378, 0.725311);
		Pd.emissiveColor.setValue(0.0465862, 0.0816971, 0.14);
		Pd.shininess.setValue(0.11);
		Pd.transparency.setValue(0.50);

		// selected
		Ps.ambientColor.setValue(0.0239312, 0.049939, 0.126667);
		Ps.diffuseColor.setValue(0.0944655, 0.197128, 0.5);
		Ps.specularColor.setValue(0.933564, 0.375326, 1);
		Ps.emissiveColor.setValue(0.171523, 0.373538, 0.62);
		Ps.shininess.setValue(0.17);
		Ps.transparency.setValue(0.50);

   		// layer deactivated
		Pl.ambientColor.setValue(0, 0, 0);
		Pl.diffuseColor.setValue(0.140388, 0.288152, 0.45);
		Pl.specularColor.setValue(1, 0.256361, 0.956754);
		Pl.emissiveColor.setValue(0.499237, 0.475195, 0.56);
		Pl.shininess.setValue(0.2);
        Pl.transparency.setValue(0.5);
	} 
	else
	if (userColor == orange) // orange scheme
	{
		// selected
		Ps.ambientColor.setValue(0.101333, 0.0523371, 0.0274005);
		Ps.diffuseColor.setValue(0.4, 0.206594, 0.10816);
		Ps.specularColor.setValue(0.923602, 1, 0.716057);
		Ps.emissiveColor.setValue(0.38, 0.235238, 0.144762);
		Ps.shininess.setValue(0.17);
		Ps.transparency.setValue(0.5);
	} else
	if (userColor == green) // green scheme
	{
		// selected
		Ps.ambientColor.setValue(0.0294127, 0.0506666, 0.0247711);
		Ps.diffuseColor.setValue(0.116103, 0.2, 0.0977808);
		Ps.specularColor.setValue(1, 0.571429, 0.5);
		Ps.emissiveColor.setValue(0.224545, 0.32, 0.265483);
		Ps.shininess.setValue(0.17);
		Ps.transparency.setValue(0.5);
	} else
	if (userColor == red) // red scheme
	{
		// selected
		Ps.ambientColor.setValue(0.101333, 0.00566726, 0.0574585);
		Ps.diffuseColor.setValue(0.4, 0.0223708, 0.22681);
		Ps.specularColor.setValue(0.844851, 0.886423, 1);
		Ps.emissiveColor.setValue(0.6, 0.364342, 0.284838);
		Ps.shininess.setValue(0.17);
		Ps.transparency.setValue(0.5);
	}
}

void
MaterialConstants::defineConeMaterials(SoMaterial& Pw, SoMaterial& Ps, SoMaterial& Pd, SoMaterial& Ph, SoMaterial& Pl, int userID)
{
	defineTransparentMaterials(Pw,Ps,Pd,Ph,Pl, userID);
	setHighlightGridColor(Ph, userID);
	int userColor = assignUserColors(userID);
	if (userColor == blue) // blue scheme
	{
        //Working color
		Pw.ambientColor.setValue(0, 0, 0);
		Pw.diffuseColor.setValue(0.638022, 0.63342, 0.8);
		Pw.specularColor.setValue(0.475319, 0.253917, 0.5);
		Pw.emissiveColor.setValue(0.189709, 0.206117, 0.25);
		Pw.shininess.setValue(1);
		Pw.transparency.setValue(0.5);

		// layer deactivated
		Pl.ambientColor.setValue(0.0231279, 0.0503672, 0.0835999);
		Pl.diffuseColor.setValue(0.0912945, 0.198818, 0.33);
		Pl.specularColor.setValue(0.380255, 0.203134, 0.4);
		Pl.emissiveColor.setValue(0.38, 0.38, 0.38);
		Pl.shininess.setValue(0.17);
        Pl.transparency.setValue(0.5);
	} else
	if (userColor == orange) // orange scheme
	{
		//Working color
		Pw.ambientColor.setValue(0, 0, 0);
		Pw.diffuseColor.setValue(0.78, 0.59611, 0.477989);
		Pw.specularColor.setValue(0.495516, 0.310705, 0.5);
		Pw.emissiveColor.setValue(0.25, 0.179864, 0.156202);
		Pw.shininess.setValue(1);
		Pw.transparency.setValue(0.5);

		// layer deactivated
		Pl.ambientColor.setValue(0.0607999, 0.0287617, 0.00113984);
		Pl.diffuseColor.setValue(0.24, 0.113533, 0.00449939);
		Pl.specularColor.setValue(0.387757, 0.308792, 0.39);
		Pl.emissiveColor.setValue(0.38, 0.38, 0.38);
		Pl.shininess.setValue(0.17);
        Pl.transparency.setValue(0.5);
	} else
	if (userColor == green) // green scheme
	{
		//Working color
		Pw.ambientColor.setValue(0, 0, 0);
		Pw.diffuseColor.setValue(0.576845, 0.7, 0.660248);
		Pw.specularColor.setValue(0.15413, 0.414817, 0.5);
		Pw.emissiveColor.setValue(0.199552, 0.25, 0.23107);
		Pw.shininess.setValue(1);
		Pw.transparency.setValue(0.5);

		// layer deactivated
		Pl.ambientColor.setValue(0.0344363, 0.0557333, 0.0515132);
		Pl.diffuseColor.setValue(0.135933, 0.22, 0.203342);
		Pl.specularColor.setValue(0.196377, 0.4, 0.15013);
		Pl.emissiveColor.setValue(0.38, 0.38, 0.38);
		Pl.shininess.setValue(0.17);
        Pl.transparency.setValue(0.5);
	} else
	if (userColor == red) // red scheme
	{
		//Working color
		Pw.ambientColor.setValue(0, 0, 0);
		Pw.diffuseColor.setValue(0.78, 0.556689, 0.645278);
		Pw.specularColor.setValue(0.36753, 0.38342, 0.55);
		Pw.emissiveColor.setValue(0.3, 0.24078, 0.280532);
		Pw.shininess.setValue(1);
		Pw.transparency.setValue(0.5);

		// layer deactivated
		Pl.ambientColor.setValue(0.0759999, 0.0290274, 0.0606776);
		Pl.diffuseColor.setValue(0.3, 0.114582, 0.239517);
		Pl.specularColor.setValue(0.26826, 0.39, 0.338322);
		Pl.emissiveColor.setValue(0.37544, 0.35842, 0.38);
		Pl.shininess.setValue(0.17);
        Pl.transparency.setValue(0.5);
	}
}

void
MaterialConstants::defineSphereMaterials(SoMaterial& Pw, SoMaterial& Ps, SoMaterial& Pd, SoMaterial& Ph, SoMaterial& Pl, int userID)
{
	defineTransparentMaterials(Pw,Ps,Pd,Ph,Pl, userID);
	setHighlightGridColor(Ph, userID);
	int userColor = assignUserColors(userID);
	if (userColor == blue) // blue scheme
	{
        //Working color
		Pw.ambientColor.setValue(0, 0, 0);
		Pw.diffuseColor.setValue(0.583464, 0.579256, 0.77);
		Pw.specularColor.setValue(0.475319, 0.253917, 0.5);
		Pw.emissiveColor.setValue(0.431589, 0.468916, 0.65);
		Pw.shininess.setValue(1);
		Pw.transparency.setValue(0.5);

        // layer deactivated
		Pl.ambientColor.setValue(0.0339367, 0.0582798, 0.0835999);
		Pl.diffuseColor.setValue(0.133961, 0.230052, 0.33);
		Pl.specularColor.setValue(0.319482, 0.225848, 0.4);
		Pl.emissiveColor.setValue(0.38, 0.38, 0.38);
		Pl.shininess.setValue(0.17);
        Pl.transparency.setValue(0.5);
	} else
	if (userColor == orange) // orange scheme
	{
        //Working color
		Pw.ambientColor.setValue(0, 0, 0);
		Pw.diffuseColor.setValue(0.68, 0.519685, 0.416709);
		Pw.specularColor.setValue(0.495516, 0.310705, 0.5);
		Pw.emissiveColor.setValue(0.65, 0.467646, 0.406125);
		Pw.shininess.setValue(1);
		Pw.transparency.setValue(0.5);
        // layer deactivated
		Pl.ambientColor.setValue(0.0607999, 0.0261929, 0);
		Pl.diffuseColor.setValue(0.24, 0.103393, 0);
		Pl.specularColor.setValue(0.387757, 0.308792, 0.39);
		Pl.emissiveColor.setValue(0.38, 0.38, 0.38);
		Pl.shininess.setValue(0.22);
        Pl.transparency.setValue(0.5);

/*
		//NEW VALUES ONLY FOR LAB@FUTURE EVALUATION
		//Working color
		Pw.ambientColor.setValue(0, 0, 0);
		Pw.diffuseColor.setValue(0.68, 0.519685, 0.416709);
		Pw.specularColor.setValue(0.495516, 0.310705, 0.5);
		Pw.emissiveColor.setValue(0.65, 0.467646, 0.406125);
		Pw.shininess.setValue(1);
		Pw.transparency.setValue(1);

		// layer deactivated
		Pl.ambientColor.setValue(0.0607999, 0.0261929, 0);
		Pl.diffuseColor.setValue(0.24, 0.103393, 0);
		Pl.specularColor.setValue(0.387757, 0.308792, 0.39);
		Pl.emissiveColor.setValue(0.38, 0.38, 0.38);
		Pl.shininess.setValue(0.22);
        Pl.transparency.setValue(1);

		// FIXME: Delete selected colors here after evaluation!!!
		// only working and layer colors should be here
		// selected
		Ps.ambientColor.setValue(0.101333, 0.0523371, 0.0274005);
		Ps.diffuseColor.setValue(0.4, 0.206594, 0.10816);
		Ps.specularColor.setValue(0.923602, 1, 0.716057);
		Ps.emissiveColor.setValue(0.38, 0.235238, 0.144762);
		Ps.shininess.setValue(0.17);
		Ps.transparency.setValue(0.7);
*/
	} else
	if (userColor == green) // green scheme
	{
        //Working color
		Pw.ambientColor.setValue(0, 0, 0);
		Pw.diffuseColor.setValue(0.593326, 0.72, 0.679112);
		Pw.specularColor.setValue(0.15413, 0.414817, 0.5);
		Pw.emissiveColor.setValue(0.494889, 0.62, 0.573054);
		Pw.shininess.setValue(1);
		Pw.transparency.setValue(0.5);

        // layer deactivated
		Pl.ambientColor.setValue(0.0428006, 0.0557333, 0.0536232);
		Pl.diffuseColor.setValue(0.16895, 0.22, 0.211671);
		Pl.specularColor.setValue(0.196377, 0.4, 0.15013);
		Pl.emissiveColor.setValue(0.38, 0.38, 0.38);
		Pl.shininess.setValue(0.17);
        Pl.transparency.setValue(0.5);
	} else
	if (userColor == red) // red scheme
	{
        //Working color
		Pw.ambientColor.setValue(0, 0, 0);
		Pw.diffuseColor.setValue(0.7, 0.499593, 0.579096);
		Pw.specularColor.setValue(0.36753, 0.38342, 0.55);
		Pw.emissiveColor.setValue(0.61, 0.489586, 0.570415);
		Pw.shininess.setValue(1);
		Pw.transparency.setValue(0.5);

        // layer deactivated
		Pl.ambientColor.setValue(0.0725, 0.0276907, 0.0578833);
		Pl.diffuseColor.setValue(0.29, 0.110763, 0.231533);
		Pl.specularColor.setValue(0.26826, 0.39, 0.338322);
		Pl.emissiveColor.setValue(0.37544, 0.35842, 0.38);
		Pl.shininess.setValue(0.22);
        Pl.transparency.setValue(0.5);
	}
}

void
MaterialConstants::defineCylinderMaterials(SoMaterial& Pw, SoMaterial& Ps, SoMaterial& Pd, SoMaterial& Ph, SoMaterial& Pl, int userID)
{
	defineTransparentMaterials(Pw,Ps,Pd,Ph,Pl, userID);
	setHighlightGridColor(Ph, userID);
	int userColor = assignUserColors(userID);
	if (userColor == blue) // blue scheme
	{
        //Working color
		Pw.ambientColor.setValue(0, 0, 0);
		Pw.diffuseColor.setValue(0.614096, 0.609667, 0.77);
		Pw.specularColor.setValue(0.475319, 0.253917, 0.5);
		Pw.emissiveColor.setValue(0.455303, 0.494681, 0.6);
		Pw.shininess.setValue(1);
		Pw.transparency.setValue(0.5);

        //inactive layer
        Pl.ambientColor.setValue(0.0282426, 0.0551149, 0.0835999);
		Pl.diffuseColor.setValue(0.111484, 0.217559, 0.33);
		Pl.specularColor.setValue(0.439068, 0.263381, 0.5);
		Pl.emissiveColor.setValue(0.38, 0.38, 0.38);
		Pl.shininess.setValue(0.17);
        Pl.transparency.setValue(0.5);
	} else
	if (userColor == orange) // orange scheme
	{
        //Working color
		Pw.ambientColor.setValue(0, 0, 0);
		Pw.diffuseColor.setValue(0.77, 0.588467, 0.471861);
		Pw.specularColor.setValue(0.495516, 0.310705, 0.5);
		Pw.emissiveColor.setValue(0.6, 0.431673, 0.374885);
		Pw.shininess.setValue(1);
		Pw.transparency.setValue(0.5);

        //inactive layer
        Pl.ambientColor.setValue(0.0607999, 0.0291829, 0.00271201);
		Pl.diffuseColor.setValue(0.24, 0.115196, 0.0107053);
		Pl.specularColor.setValue(0.387757, 0.308792, 0.39);
		Pl.emissiveColor.setValue(0.38, 0.38, 0.38);
		Pl.shininess.setValue(0.17);
        Pl.transparency.setValue(0.5);
	} else
	if (userColor == green) // green scheme
	{
        //Working color
		Pw.ambientColor.setValue(0, 0, 0);
		Pw.diffuseColor.setValue(0.576845, 0.7, 0.660248);
		Pw.specularColor.setValue(0.15413, 0.414817, 0.5);
		Pw.emissiveColor.setValue(0.478925, 0.6, 0.554568);
		Pw.shininess.setValue(1);
		Pw.transparency.setValue(0.5);

        //inactive layer
        Pl.ambientColor.setValue(0.0428006, 0.0557333, 0.0536232);
		Pl.diffuseColor.setValue(0.16895, 0.22, 0.211671);
		Pl.specularColor.setValue(0.196377, 0.4, 0.15013);
		Pl.emissiveColor.setValue(0.38, 0.38, 0.38);
		Pl.shininess.setValue(0.17);
        Pl.transparency.setValue(0.5);
	} else
	if (userColor == red) // red scheme
	{
        //Working color
		Pw.ambientColor.setValue(0, 0, 0);
		Pw.diffuseColor.setValue(0.71, 0.50673, 0.587369);
		Pw.specularColor.setValue(0.36753, 0.38342, 0.55);
		Pw.emissiveColor.setValue(0.61, 0.489586, 0.570415);
		Pw.shininess.setValue(1);
		Pw.transparency.setValue(0.5);

        //inactive layer
        Pl.ambientColor.setValue(0.0734666, 0.0280599, 0.058655);
		Pl.diffuseColor.setValue(0.29, 0.110763, 0.231533);
		Pl.specularColor.setValue(0.26826, 0.39, 0.338322);
		Pl.emissiveColor.setValue(0.37544, 0.35842, 0.38);
		Pl.shininess.setValue(0.17);
        Pl.transparency.setValue(0.5);
	}
}

void
MaterialConstants::defineCubeMaterials(SoMaterial& Pw, SoMaterial& Ps, SoMaterial& Pd, SoMaterial& Ph, SoMaterial& Pl, int userID)
{
	defineTransparentMaterials(Pw,Ps,Pd,Ph,Pl, userID);
	setHighlightGridColor(Ph, userID);
	int userColor = assignUserColors(userID);
	if (userColor == blue) // blue scheme
	{
        //Working color
		Pw.ambientColor.setValue(0, 0, 0);
		Pw.diffuseColor.setValue(0.833984, 0.86141, 1);
		Pw.specularColor.setValue(0, 0, 0);
		Pw.emissiveColor.setValue(0.46538, 0.472892, 0.57);
		Pw.shininess.setValue(1);
		Pw.transparency.setValue(0.4);
		
		// highlight color
		Ph.emissiveColor.setValue(0, 0.315, 0.7);

        //inactive layer
        Pl.ambientColor.setValue(0.0401761, 0.0535239, 0.0861332);
		Pl.diffuseColor.setValue(0.15859, 0.211279, 0.34);
		Pl.specularColor.setValue(0, 0, 0);
		Pl.emissiveColor.setValue(0.38, 0.38, 0.38);
		Pl.shininess.setValue(0.17);
        Pl.transparency.setValue(0.5);
	} else
	if (userColor == orange) // orange scheme
	{
        //Working color
		Pw.ambientColor.setValue(0, 0, 0);
		Pw.diffuseColor.setValue(1, 0.817816, 0.723168);
		Pw.specularColor.setValue(0, 0, 0);
		Pw.emissiveColor.setValue(0.5, 0.370461, 0.285278);
		Pw.shininess.setValue(1);
		Pw.transparency.setValue(0.4);

		// highlight color
		Ph.emissiveColor.setValue(0.9, 0.510321, 0.0503342);

        //inactive layer
        Pl.ambientColor.setValue(0.0759999, 0.0364787, 0.00339);
		Pl.diffuseColor.setValue(0.3, 0.143995, 0.0133816);
		Pl.specularColor.setValue(0, 0, 0);
		Pl.emissiveColor.setValue(0.38, 0.38, 0.38);
		Pl.shininess.setValue(0.17);
        Pl.transparency.setValue(0.5);
	} else
	if (userColor == green) // green scheme
	{
        //Working color
		Pw.ambientColor.setValue(0, 0, 0);
		Pw.diffuseColor.setValue(0.782861, 0.95, 0.896051);
		Pw.specularColor.setValue(0, 0, 0);
		Pw.emissiveColor.setValue(0.461476, 0.53, 0.479837);
		Pw.shininess.setValue(1);
		Pw.transparency.setValue(0.4);

		// highlight color
		Ph.emissiveColor.setValue(0.674137, 0.78, 0.440404);

        //inactive layer
        Pl.ambientColor.setValue(0.0367913, 0.0557333, 0.0525684);
		Pl.diffuseColor.setValue(0.145229, 0.22, 0.207507);
		Pl.specularColor.setValue(0, 0, 0);
		Pl.emissiveColor.setValue(0.38, 0.38, 0.38);
		Pl.shininess.setValue(0.17);
        Pl.transparency.setValue(0.5);
	} else
	if (userColor == red) // red scheme
	{
        //Working color
		Pw.ambientColor.setValue(0, 0, 0);
		Pw.diffuseColor.setValue(1, 0.8026, 0.935107);
		Pw.specularColor.setValue(0, 0, 0);
		Pw.emissiveColor.setValue(0.5, 0.335725, 0.401979);
		Pw.shininess.setValue(1);
		Pw.transparency.setValue(0.4);

		// highlight color
		Ph.emissiveColor.setValue(1, 0.351688, 0.730279);

        //inactive layer
        Pl.ambientColor.setValue(0.0785332, 0.0299949, 0.0627002);
		Pl.diffuseColor.setValue(0.31, 0.118401, 0.247501);
		Pl.specularColor.setValue(0, 0, 0);
		Pl.emissiveColor.setValue(0.38, 0.38, 0.38);
		Pl.shininess.setValue(0.17);
        Pl.transparency.setValue(0.5);
	}
}

// sets the color which is used for the wireframe grid when
// highlighting the nearest object
void
MaterialConstants::setHighlightGridColor(SoMaterial& Ph, int userID)
{
	int userColor = assignUserColors(userID);
	if (userColor == blue) // blue scheme
	{
		// highlight grid color
        Ph.ambientColor.setValue(0, 0, 0);
		Ph.diffuseColor.setValue(0, 0.315, 0.7);
		Ph.specularColor.setValue(0, 0, 0);
		Ph.emissiveColor.setValue(0, 0, 0);
		Ph.shininess.setValue(0.2);
		Ph.transparency.setValue(0);

	} else
	if (userColor == orange) // orange scheme
	{
		// highlight grid color  
		Ph.ambientColor.setValue(0, 0, 0);
		Ph.diffuseColor.setValue(0.63, 0.364357, 0.0900688);
		Ph.specularColor.setValue(0, 0, 0);
		Ph.emissiveColor.setValue(0, 0, 0);
		Ph.shininess.setValue(0);
		Ph.transparency.setValue(0);
	} else
	if (userColor == green) // green scheme
	{
		// highlight grid color  
		Ph.ambientColor.setValue(0, 0, 0);
		Ph.diffuseColor.setValue(0.41921, 0.5, 0.272846);
		Ph.specularColor.setValue(0, 0, 0);
		Ph.emissiveColor.setValue(0, 0, 0);
		Ph.shininess.setValue(0);
		Ph.transparency.setValue(0);
	} else
	if (userColor == red) // red scheme
	{
		// highlight grid color  
		Ph.ambientColor.setValue(0, 0, 0);
		Ph.diffuseColor.setValue(0.63, 0.22242, 0.293974);
		Ph.specularColor.setValue(0, 0, 0);
		Ph.emissiveColor.setValue(0, 0, 0);
		Ph.shininess.setValue(0);
		Ph.transparency.setValue(0);
	}
}

// set and change Plane texture
void
MaterialConstants::changePlaneTexture(SoPlaneKit* plane, int state, int currentUserID)
{
	SoTexture2* planeTex = (SoTexture2*) plane->getPart("texture", false);//texture.getValue();
	// The texture color replaces the surface color
	planeTex->model.setValue(SoTexture2::DECAL);
	int userColor = assignUserColors(currentUserID);
	if (userColor == 0) // blue scheme
	{
		switch(state)
		{
		case 0: // ACTIVE - Working color
			{
				planeTex->filename.setValue("./textures/blue_active_512.jpg");
				break;
			}
		case 1: // SELECTED
			{
				planeTex->filename.setValue("./textures/blue_selected_512.jpg");
				break;
			}
		case 2: // INACTIVE
			{
				planeTex->filename.setValue("./textures/blue_inactive_512.jpg");
				break;
			}
		}
	} else
	if (userColor == 1) // orange scheme
	{
		switch(state)
		{
		case 0: // ACTIVE - Working color
			{
				planeTex->filename.setValue("./textures/orange_active_512.jpg");
				break;
			}
		case 1: // SELECTED
			{
				planeTex->filename.setValue("./textures/orange_selected_512.jpg");
				break;
			}
		case 2: // INACTIVE
			{
				planeTex->filename.setValue("./textures/orange_inactive_512.jpg");
				break;
			}
		}
	} else
	if (userColor == 2) // green scheme
	{
		switch(state)
		{
		case 0: // ACTIVE - Working color
			{
				planeTex->filename.setValue("./textures/green_active_512.jpg");
				break;
			}
		case 1: // SELECTED
			{
				planeTex->filename.setValue("./textures/green_selected_512.jpg");
				break;
			}
		case 2: // INACTIVE
			{
				planeTex->filename.setValue("./textures/green_inactive_512.jpg");
				break;
			}
		}
	} else
	if (userColor == 3) // red scheme
	{
		switch(state)
		{
		case 0: // ACTIVE - Working color
			{
				planeTex->filename.setValue("./textures/red_active_512.jpg");
				break;
			}
		case 1: // SELECTED
			{
				planeTex->filename.setValue("./textures/red_selected_512.jpg");
				break;
			}
		case 2: // INACTIVE
			{
				planeTex->filename.setValue("./textures/red_inactive_512.jpg");
				break;
			}
		}
	}			
}

// set and change Cube texture
void
MaterialConstants::changeCubeTexture(SoCubeKit* cube, int state, int currentUserID)
{
	SoTexture2* cubeTex = (SoTexture2*) cube->getPart("texture", false);//texture.getValue();
	// The texture color replaces the surface color
	cubeTex->model.setValue(SoTexture2::DECAL);
	int userColor = assignUserColors(currentUserID);
	if (userColor == 0) // blue scheme
	{
		switch(state)
		{
		case 0: // ACTIVE - Working color
			{
				cubeTex->filename.setValue("./textures/cubeblue_active_512.jpg");
				break;
			}
		case 1: // SELECTED
			{
				cubeTex->filename.setValue("./textures/cubeblue_selected_512.jpg");
				break;
			}
		case 2: // INACTIVE
			{
				cubeTex->filename.setValue("./textures/cubeblue_inactive_512.jpg");
				break;
			}
		}
	} 
	else
	if (userColor == 1) // orange scheme
	{
		switch(state)
		{
		case 0: // ACTIVE - Working color
			{
				cubeTex->filename.setValue("./textures/cubeorange_active_512.jpg");
				break;
			}
		case 1: // SELECTED
			{
				cubeTex->filename.setValue("./textures/cubeorange_selected_512.jpg");
				break;
			}
		case 2: // INACTIVE
			{
				cubeTex->filename.setValue("./textures/cubeorange_inactive_512.jpg");
				break;
			}
		}
	} else
	if (userColor == 2) // green scheme
	{
		switch(state)
		{
		case 0: // ACTIVE - Working color
			{
				cubeTex->filename.setValue("./textures/cubegreen_active_512.jpg");
				break;
			}
		case 1: // SELECTED
			{
				cubeTex->filename.setValue("./textures/cubegreen_selected_512.jpg");
				break;
			}
		case 2: // INACTIVE
			{
				cubeTex->filename.setValue("./textures/cubegreen_inactive_512.jpg");
				break;
			}
		}
	}
	else
	if (userColor == 3) // red scheme
	{
		switch(state)
		{
		case 0: // ACTIVE - Working color
			{
				cubeTex->filename.setValue("./textures/cubered_active_512.jpg");
				break;
			}
		case 1: // SELECTED
			{
				cubeTex->filename.setValue("./textures/cubered_selected_512.jpg");
				break;
			}
		case 2: // INACTIVE
			{
				cubeTex->filename.setValue("./textures/cubered_inactive_512.jpg");
				break;
			}
		}
	}	
}
