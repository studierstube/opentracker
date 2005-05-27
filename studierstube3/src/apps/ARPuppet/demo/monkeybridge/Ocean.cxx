/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/* Ocean.cxx contains the implementation of Ocean
 *
 * This code is based on the program of Carsten Dachsbacher, 2001
 *
 * @author Istvan Barakonyi
 *
 * $Id: Ocean.cxx 4209 2005-05-19 09:06:39Z bara $
 * @file
 */
/* ======================================================================== */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

// ...

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

// time function for random seed generation
#include <time.h>

#include "Ocean.h"

SO_NODE_SOURCE(Ocean);

void Ocean::initClass()
{
    SO_NODE_INIT_CLASS(Ocean, SoNode, "Node");
}

Ocean::Ocean()
{
    SO_NODE_CONSTRUCTOR(Ocean);

    SO_NODE_ADD_FIELD(waveResolution, (256));
    SO_NODE_ADD_FIELD(meshResolution, (SbVec2f(64,64)));
    SO_NODE_ADD_FIELD(windSpeed, (4.0f));
    SO_NODE_ADD_FIELD(windDirection, (0.0f));
    SO_NODE_ADD_FIELD(gravity, (9.81f));
    SO_NODE_ADD_FIELD(alpha, (0.0081f));
    SO_NODE_ADD_FIELD(speed, (700.0f));
    SO_NODE_ADD_FIELD(color, (1,1,1));
    SO_NODE_ADD_FIELD(transparency, (0.0f));
    SO_NODE_ADD_FIELD(scale, (1.0f));

    initialized=false;

    meshResolutionSensor=new SoFieldSensor(initCB,this);
    waveResolutionSensor=new SoFieldSensor(initCB,this);
    windSpeedSensor=new SoFieldSensor(initCB,this);
    windDirectionSensor=new SoFieldSensor(initCB,this);
    gravitySensor=new SoFieldSensor(initCB,this);

    meshResolutionSensor->attach(&meshResolution);
    waveResolutionSensor->attach(&waveResolution);
    windSpeedSensor->attach(&windSpeed);
    windDirectionSensor->attach(&windDirection);
    gravitySensor->attach(&gravity);

    // we haven't initialized this component yet
    initialized=false;

    // seed for random number generator
    srand((unsigned)time(NULL));

    // init water data arrays
    wave=NULL;
    waterNormal=NULL;
    waterHeight=NULL;

    // animation loop timer -> time sensor: 1/50sec, start immediately
    timerSensor=new SoTimerSensor(timerSensorCB,this);
    timerSensor->setInterval(1.0/50.0);

    // init water data structure
    initCB(this,NULL);
}

Ocean::~Ocean()
{
    if (timerSensor->isScheduled()) timerSensor->unschedule();
    delete timerSensor;

    delete meshResolutionSensor;
    delete waveResolutionSensor;
    delete windSpeedSensor;
    delete windDirectionSensor;
    delete gravitySensor;
    
    delete[]waterHeight;
    delete[]waterNormal;
    delete[]wave;
}

void Ocean::GLRender(SoGLRenderAction *action)
{
    if (this->initialized) this->animate();
}

void Ocean::initCB(void *data, SoSensor *sensor)
{
    Ocean *ocean=(Ocean*)data;
    
    if (ocean->timerSensor->isScheduled()) ocean->timerSensor->unschedule();

    ocean->initialized=false;
    SbVec2f mr=ocean->meshResolution.getValue();

    // if mesh resolution changed or very first init, resize water height and normal array
    if (sensor==ocean->meshResolutionSensor || sensor==NULL){
        if (ocean->waterHeight!=NULL) delete[]ocean->waterHeight;
        if (ocean->waterNormal!=NULL) delete[]ocean->waterNormal;
        ocean->waterHeight=new float[(int)(mr[0]*mr[1])];
        ocean->waterNormal=new SbVec3f[(int)(mr[0]*mr[1])];

        // init water normals
        for (int i=0;i<(int)(mr[0]*mr[1]);i++){
            ocean->waterNormal[i][0]=0.0f;
            ocean->waterNormal[i][1]=1.0f;
            ocean->waterNormal[i][2]=0.0f;
        }
    }

    // initialize wave parameters
    if (ocean->wave!=NULL) delete[]ocean->wave;
    ocean->wave=new WAVE[ocean->waveResolution.getValue()];
    WAVE *w=ocean->wave;
    ocean->nWaves=0;

    // frequency distribution
    ocean->freqPeak=0.13f*ocean->gravity.getValue()/ocean->windSpeed.getValue();

    while (ocean->nWaves<16){

        float frequency=ocean->freqPeak+rand()/16384.0f*0.5f-0.5f; //RAND_MAX=32767, RAND_MAX/2.0=16383.5f

        if (frequency>0.0f){
            w->lambda=2.0f*(float)M_PI/((float)pow(2.0f*M_PI*frequency,2.0f)/ocean->gravity.getValue());
            w->k=2.0f*(float)M_PI/w->lambda;
            w->omega=(float)sqrt(ocean->gravity.getValue()*w->k);
            w->freq=w->omega/(2.0f*(float)M_PI);
            w->period=1.0f/w->freq;
            w->direction=0.0f+(rand()/16384.0f+0.5f)*(float)M_PI;
            w->dirX=(float)cos(w->direction)*0.5f;
            w->dirY=(float)sin(w->direction)*0.5f;

            float phi0=rand()/16384.0f*(float)M_PI;
            float a0=ocean->waveAmplitude(w->freq,w->direction-ocean->windDirection.getValue(),w->k);
            w->amplitude=a0*(float)cos(phi0);
            w->phase=a0*(float)sin(phi0);

            if (fabs(w->amplitude)>=0.0001f){
                w++;
                ocean->nWaves++;
            }
        }
    }

    ocean->initialized=true;

    if (!ocean->timerSensor->isScheduled()) ocean->timerSensor->schedule();

    printf("MONKEYBRIDGE: the ocean component has been initialized\n");
}

void Ocean::timerSensorCB(void *data, SoSensor *sensor)
{
    ((Ocean*)data)->touch();
}

void Ocean::animate()
{
    double time=(double)clock()/speed.getValue();

    SbVec2f mr=meshResolution.getValue();
    for (int i=0;i<mr[1];i++){
        for (int j=0;j<mr[0];j++){
            float h=0.0f;
            for (int k=0;k<nWaves;k++){                
                h+=(wave+k)->amplitude*
                   (float)sin(
                       (wave+k)->phase+
                       (wave+k)->omega*(wave+k)->freq*(time+(wave+k)->dirX*j+
                       (wave+k)->dirY*i)
                   );
            }
            waterHeight[j+i*(int)(mr[0])]=h;
        }
    }

    for (i=0;i<mr[1];i++){
        for (int j=0;j<mr[0];j++){
             float x,y,z;
             x=waterHeight[ax(j+1)+i*(int)(mr[0])]-waterHeight[ax(j-1)+i*(int)(mr[0])];
             z=waterHeight[j+ay(i+1)*(int)(mr[0])]-waterHeight[j+ay(i-1)*(int)(mr[0])];
             y=4.0f;

             waterNormal[j+i*(int)(mr[0])][0]=x;
             waterNormal[j+i*(int)(mr[0])][1]=y;
             waterNormal[j+i*(int)(mr[0])][2]=z;
        }
    }

    SbColor col=color.getValue();
    glColor4f(col[0],col[1],col[2],transparency.getValue());

    for (i=0;i<mr[1];i++){             
        glBegin(GL_QUAD_STRIP);
        for (int j=0;j<mr[0];j++){
            int x=j&((int)(mr[0])-1);
            int y=i&((int)(mr[1])-1);
            int y1=(i+1)&((int)(mr[1])-1);

            glNormal3fv((float*)&waterNormal[x+y*(int)(mr[0])]);
            //glTexCoord2f(i*0.3f,j*0.3f);
            glVertex3f(
                       (float)(j-(int)(mr[0])/2)*scale.getValue(), 
                       waterHeight[x+y*(int)(mr[0])]*scale.getValue(),
                       (float)(i-(int)(mr[1])/2)*scale.getValue() 
                      );
            
            glNormal3fv((float*)&waterNormal[x+y1*(int)(mr[0])]);
            //glTexCoord2f(i*0.3f,j*0.3f+0.3f);
            glVertex3f(
                       (float)(j-(int)(mr[0])/2)*scale.getValue(), 
                       waterHeight[x+y1*(int)(mr[0])]*scale.getValue(),
                       (float)(i+1-(int)(mr[1])/2)*scale.getValue()
                      );
        }
        glEnd();

        /*
        // TODO; losing performance with vertex arrays ?!!

        into header file: 
        -----------------

            VERTEX *mesh;

            where VERTEX has the following structure:

                typedef struct {
                    //GLfloat t[2];
	                GLfloat n[3];
	                GLfloat v[3];
                } VERTEX;

        into the source code instead of the glBegin(GL_QUAD_STRIP) ... glEnd() block:
        -----------------------------------------------------------------------------

        VERTEX *m=mesh;
        for (int j=0;j<mr[0];j++){
            int x=j&((int)(mr[0])-1);
            int y=i&((int)(mr[1])-1);
            int y1=(i+1)&((int)(mr[1])-1);

            int k;
            for (k=0;k<3;k++) m->n[k]=waterNormal[x+y*(int)(mr[0])][k];
            m->v[0]=(float)(j-(int)(mr[0])/2);
            m->v[1]=waterHeight[x+y*(int)(mr[0])];
            m->v[2]=(float)(i-(int)(mr[1])/2);
            m++;
            for (k=0;k<3;k++) m->n[k]=waterNormal[x+y1*(int)(mr[0])][k];
            m->v[0]=(float)(j-(int)(mr[0])/2);
            m->v[1]=waterHeight[x+y1*(int)(mr[0])];
            m->v[2]=(float)(i+1-(int)(mr[1])/2);
            m++;
        }
        glInterleavedArrays(GL_N3F_V3F,0,mesh);
        glDrawArrays(GL_QUAD_STRIP,0,(int)mr[0]*2);
        */
    }
}

int Ocean::ax(int x)
{
    SbVec2f mr=meshResolution.getValue();

    return (x<0) ? x+(int)mr[0]: (x>=(int)mr[0] ? x-(int)mr[0] : x);
}

int Ocean::ay(int y)
{
    SbVec2f mr=meshResolution.getValue();

    return (y<0) ? y+(int)mr[1] : (y>=(int)mr[1] ? y-(int)mr[1] : y);
}

float Ocean::gamma(float y)
{
    float integral=0.0f;

    for(int i=0;i<100.0f;i++){
        float x=i/100.0f*5.0f;
        integral+=(float)pow(x,y-1)*(float)exp(-x)*1.0f/100.0f*5.0f;
    }

    return integral;
}

float Ocean::directionEnergy(float f,float theta)
{
    float temp=(float)cos(theta*0.5f);
    temp*=temp;

    float s_sm,sm;
    if (f>=freqPeak){
        s_sm=(float)pow(f/freqPeak,-2.5f);
        sm=9.77f;
    } 
    else {
        s_sm=(float)pow(f/freqPeak,5.0f);
        sm=6.97f;
    }
    float s=s_sm*sm;

    return gamma(s+1)*(float)pow(temp,s)/(2.0f*(float)sqrt(M_PI)*gamma(s+0.5f));
}

float Ocean::energy1DFreq(float f)
{
    return alpha.getValue()*
           gravity.getValue()*gravity.getValue()/((float)pow(2.0f*M_PI,4)*(float)pow(f,5))*
           (float)exp(-5.0f/4.0f*(float)pow((f/freqPeak),-4.0f));
}

float Ocean::energy2DFreqAngle(float f,float theta)
{
    return energy1DFreq(f)*directionEnergy(f,theta);
}

float Ocean::waveAmplitude(float f,float theta,float k)
{
    //omega = area of the water patch
    float omega=10.0f;
    return (float)sqrt(energy2DFreqAngle(f,theta)*gravity.getValue()*M_PI*M_PI/(k*f*omega));
}
