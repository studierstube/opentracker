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
/* BoidFlock.h contains the definition of class BoidFlock
 *
 * @author Istvan Barakonyi
 *
 * $Id: BoidFlock.cxx 3783 2004-09-28 12:45:10Z bara $
 * @file
 */
/* ======================================================================== */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include "Inventor/nodes/SoSeparator.h"
#include "Inventor/nodes/SoTransform.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

// Stb classes
#include <stbapi/misc/SoFileSubgraph.h>

// own classes
#include "BoidFlock.h"

SO_KIT_SOURCE(BoidFlock);

void BoidFlock::initClass()
{
    SO_KIT_INIT_CLASS(BoidFlock, SoPuppeteerKit, "SoPuppeteerKit");
}

BoidFlock::BoidFlock()
{
    SO_KIT_CONSTRUCTOR(BoidFlock);

    SO_KIT_ADD_FIELD(filename, (""));
    SO_KIT_ADD_FIELD(number, (0));

    SO_KIT_ADD_FIELD(influenceRadiusSquared, (0.0f));
    SO_KIT_ADD_FIELD(collisionFraction, (0.0f));
    SO_KIT_ADD_FIELD(angleTweak, (0.0f));
    SO_KIT_ADD_FIELD(pitchToSpeedRatio, (0.0f));
    SO_KIT_ADD_FIELD(normalSpeed, (0.0f));
    
    SO_KIT_ADD_FIELD(goal, (0.0f,0.0f,0.0f));
    SO_KIT_ADD_FIELD(obstaclePosition, (0,0,0));
    SO_KIT_ADD_FIELD(obstacleRadius, (0));

    SO_KIT_INIT_INSTANCE();

    // no commands defined --> TODO: does it make sense to script them?
    commandString="";

    // create field sensors
    filenameSensor=new SoFieldSensor(generateFlockCB,this);
    numberSensor=new SoFieldSensor(generateFlockCB,this);

    // animation loop timer -> time sensor: 1/50sec, start immediately
    timerSensor=new SoTimerSensor(timerSensorCB,this);
    timerSensor->setInterval(1.0f/50.0f);

    setUpConnections(TRUE,TRUE);
}

BoidFlock::~BoidFlock()
{
    for (int i=0;i<boidList.getLength();i++) boidList.remove(0);

    delete filenameSensor;
    delete numberSensor;
}

SbBool BoidFlock::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;

    if (onOff) {
        filenameSensor->attach(&filename);
        numberSensor->attach(&number);

        generateFlockCB(this,NULL);

        if (!timerSensor->isScheduled()) timerSensor->schedule();
    }
    else {
        if (timerSensor->isScheduled()) timerSensor->unschedule();

        filenameSensor->detach();
        numberSensor->detach();
    }
    
    // set up connections in parent class SoPuppeteerKit
    SoPuppeteerKit::setUpConnections(onOff,doItAlways);

    return !(connectionsSetUp = onOff);
}

void BoidFlock::generateFlockCB(void *data,SoSensor *sensor)
{
    BoidFlock *bf=(BoidFlock*)data;

    // remove all boids and associated data remaining from the previously constructed flock
    for (;bf->getPuppetNum();) {
        bf->removePuppet(0);
    }
    for (;bf->transformList.getLength();) bf->transformList.remove(0);

    // check valid boid number
    if (bf->number.getValue()<0) return;

    // load boid from file
    SoFileSubgraph *boidFile=new SoFileSubgraph();
    boidFile->fileName=bf->filename.getValue();

    // add new boids
    for (int i=0;i<bf->number.getValue();i++){
        // build scene graph
        SoSeparator *boidRoot=new SoSeparator;
        SoTransform *boidTrans=new SoTransform;
        bf->transformList.append(boidTrans);
        boidRoot->addChild(boidTrans);
        boidRoot->addChild(boidFile);
        bf->addPuppet(boidRoot);
    }

    // modify the size of data arrays if necessary
    if (bf->boidList.getLength()<bf->number.getValue()){
        int max=bf->number.getValue()-bf->boidList.getLength();
        for (int j=0;j<max;j++) {
            Boid *b=new Boid;
            b->loc.setValue(-10.0f+20.0f*bf->rnd(),10.0f*bf->rnd(),-10.0f+20.0f*bf->rnd());
            b->dir.setValue(-1.0f+2.0f*bf->rnd(),-1.0f+2.0f*bf->rnd(),-1.0f+2.0f*bf->rnd());
            b->yaw=b->pitch=b->roll=b->dyaw=0.0f;
            b->speed=bf->normalSpeed.getValue();
            bf->boidList.append(b);
            float *dist=new float(0);
            bf->distanceList.append(dist);
        }
    }
    else {
        int max=bf->boidList.getLength()-bf->number.getValue();
        for (int j=0;j<max;j++) bf->boidList.remove(bf->boidList.getLength()-1);
    }
}

void BoidFlock::timerSensorCB(void *data, SoSensor *sensor)
{
    BoidFlock *bf=(BoidFlock*)data;
    
    bf->animate();

    for (int i=0;i<bf->boidList.getLength();i++){
        SoTransform *trans=(SoTransform*)(bf->transformList[i]);
        trans->translation.setValue(
            bf->getBoid(i)->loc[0]/100.0f,
            bf->getBoid(i)->loc[1]/100.0f,
            bf->getBoid(i)->loc[2]/100.0f
        );
        SbVec3f t=trans->translation.getValue();
        trans->rotation.setValue(SbRotation(SbVec3f(0,0,1),bf->getBoid(i)->dir));
    }

    bf->touch();
}

float BoidFlock::getBoidDistance(int num1,int num2)
{
    if (num1<boidList.getLength() && num2<boidList.getLength()) {
        return *((float*)(distanceList[num1*number.getValue()+num2]));
    }
    else return 0.0f;
}

void BoidFlock::setBoidDistance(int num1,int num2,float dist)
{
    if (num1<boidList.getLength() && num2<boidList.getLength()) {
        float *value=new float(dist);
        distanceList[num1*number.getValue()+num2]=value;
    }
}

Boid* BoidFlock::getBoid(int num)
{
    if (num<boidList.getLength()) return (Boid*)(boidList[num]);
    else return NULL;
}

float BoidFlock::rnd(float max)
{
    return (rand()/(float)RAND_MAX)*fabs(max);
}

void BoidFlock::animate()
{
    float distSquared;
    int i,j;

    // first update the boid distance array with values between 0.0 and 1.0 (with 0.0 being furthest away)
    for (i=0;i<number.getValue();i++) {
        for (j=i+1;j<number.getValue();j++) {
            SbVec3f distVec=getBoid(i)->loc-getBoid(j)->loc;
            distSquared=distVec[0]*distVec[0]+distVec[1]*distVec[1]+distVec[2]*distVec[2];
            distSquared=influenceRadiusSquared.getValue()-distSquared;
            if (distSquared<0.0f) distSquared=0.0f;
            else distSquared/=influenceRadiusSquared.getValue();
            setBoidDistance(i,j,distSquared);
            setBoidDistance(j,i,distSquared);
        }
        setBoidDistance(i,i,0);
        getBoid(i)->delta_dir=SbVec3f(0,0,0);
        getBoid(i)->delta_pos=SbVec3f(0,0,0);
        getBoid(i)->delta_cnt=0;
    }

    float invCollisionFraction=1.0f/(1.0f-collisionFraction.getValue());

    for (i=0;i<number.getValue();i++) {
        for (j=i+1;j<number.getValue();j++) {
            // if i is near j, let them influence each other
            if (getBoidDistance(i,j)>0.0f) {
                SbVec3f diff=getBoid(i)->loc-getBoid(j)->loc; 
                diff.normalize();
                SbVec3f delta;
                float col=0.0f; // collision weighting

                // only do collision testing against the nearest ones
                if (getBoidDistance(i,j)-collisionFraction.getValue()>0.0f) {
                    col=(getBoidDistance(i,j)-collisionFraction.getValue())*invCollisionFraction;
                }

                // add in a little flock centering
                if (getBoidDistance(i,j)-(1.0f-collisionFraction.getValue())>0.0f) {
                    col-=getBoidDistance(i,j)*(1.0f-col);
                }
                delta=col*diff;

                // add in the collision avoidance
                getBoid(i)->delta_pos+=delta;
                getBoid(j)->delta_pos-=delta;

                // add in the velocity influences
                getBoid(i)->delta_dir+=getBoid(j)->dir*getBoidDistance(i,j);
                getBoid(j)->delta_dir+=getBoid(i)->dir*getBoidDistance(i,j);
                getBoid(i)->delta_cnt++;
                getBoid(j)->delta_cnt++;
            }
        }
    }

    // update the boids
    for (i=0;i<number.getValue();i++) {
        if (getBoid(i)->delta_cnt) {
            getBoid(i)->delta_dir/=(float)getBoid(i)->delta_cnt;
            getBoid(i)->delta_dir-=getBoid(i)->dir;
            getBoid(i)->delta_dir*=1.5f;
        }
        SbVec3f delta=getBoid(i)->delta_dir+getBoid(i)->delta_pos;
        SbVec3f offset;

        // add in the influence of the global goal
        SbVec3f flockGoal=goal.getValue()*100.0f;
        flockGoal-=getBoid(i)->loc;
        flockGoal.normalize();
        flockGoal*=0.5f;
        delta+=flockGoal;

        // add in any obstacles
        for (j=0;j<obstaclePosition.getNum();j++) {
            SbVec3f ob=getBoid(i)->loc-obstaclePosition[j];
            float radius=(j<obstacleRadius.getNum()) ? obstacleRadius[j] : (obstacleRadius.getNum() ? obstacleRadius.getNum()-1 : 0);
            float dist=ob.length();
            if (dist>2.0f*radius) continue;
            ob.normalize();
            dist=1.0f-dist/(2.0f*radius);
            delta+=dist*ob*5.0f;
        }

        // first deal with pitch changes
        if (delta[1]>0.01f) {           // we're too low
            getBoid(i)->pitch+=angleTweak.getValue();
            if (getBoid(i)->pitch>0.8f) getBoid(i)->pitch=0.8f;
        }
        else if (delta[1]<-0.01f) {    // we're too high
            getBoid(i)->pitch-=angleTweak.getValue();
            if (getBoid(i)->pitch<-0.8f) getBoid(i)->pitch=-0.8f;
        }
        else getBoid(i)->pitch*=0.98f; // add damping

        // speed up or slow down depending on angle of attack
        getBoid(i)->speed-=getBoid(i)->pitch*pitchToSpeedRatio.getValue();
        // damp back to normal
        getBoid(i)->speed=(getBoid(i)->speed-normalSpeed.getValue())*0.99f+normalSpeed.getValue();

        if (getBoid(i)->speed<normalSpeed.getValue()/2.0f) getBoid(i)->speed=normalSpeed.getValue()/2.0f;
        if (getBoid(i)->speed>normalSpeed.getValue()*5.0f) getBoid(i)->speed=normalSpeed.getValue()*5.0f;

        // now figure out yaw changes
        offset=delta;
        offset[1]=0.0f;
        delta=getBoid(i)->dir;
        offset.normalize();
        float dot=offset.dot(delta);

        // speed up slightly if not turning much
        if (dot>0.7f) {
            dot-=0.7f;
            getBoid(i)->speed+=dot*0.005f;
        }
        offset=offset.cross(delta);
        dot=(1.0f-dot)/2.0f*0.07f;

        if (offset[1]>0.05f) getBoid(i)->dyaw=(getBoid(i)->dyaw*19.0f+dot)*0.05f;
        else if (offset[1]<-0.05f) getBoid(i)->dyaw=(getBoid(i)->dyaw*19.0f-dot)*0.05f;
        else getBoid(i)->dyaw*=0.98f; // damp it

        getBoid(i)->yaw+=getBoid(i)->dyaw;
        getBoid(i)->roll=-getBoid(i)->dyaw*20.0f;
    }

    for (i=0;i<number.getValue();i++){
        SbMatrix world,yawMatrix,pitchMatrix,rollMatrix;

        /*world.setTranslate(getBoid(i)->loc);
        yawMatrix.setRotate(SbRotation(SbVec3f(0,1,0),getBoid(i)->yaw));
        pitchMatrix.setRotate(SbRotation(SbVec3f(1,0,0),getBoid(i)->pitch));
        rollMatrix.setRotate(SbRotation(SbVec3f(0,0,1),getBoid(i)->roll));*/
        world=SbMatrix::identity();yawMatrix=SbMatrix::identity();pitchMatrix=SbMatrix::identity();rollMatrix=SbMatrix::identity();

        world[3][0]=getBoid(i)->loc[0];world[3][1]=getBoid(i)->loc[1];world[3][2]=getBoid(i)->loc[2];

        yawMatrix[0][0]=cos(getBoid(i)->yaw);yawMatrix[2][2]=cos(getBoid(i)->yaw);
        yawMatrix[0][2]=sin(getBoid(i)->yaw);yawMatrix[2][0]=-sin(getBoid(i)->yaw);

        pitchMatrix[1][1]=cos(getBoid(i)->pitch);pitchMatrix[2][2]=cos(getBoid(i)->pitch);
        pitchMatrix[1][2]=-sin(getBoid(i)->pitch);pitchMatrix[2][1]=sin(getBoid(i)->pitch);

        rollMatrix[0][0]=cos(getBoid(i)->roll);rollMatrix[1][1]=cos(getBoid(i)->roll);
        rollMatrix[0][1]=-sin(getBoid(i)->roll);rollMatrix[1][0]=sin(getBoid(i)->roll);

        //yawMatrix.multRight(pitchMatrix);
        //yawMatrix.multRight(rollMatrix);
        //world.multRight(yawMatrix);
        world = MatrixMult(
            world, 
            MatrixMult(
                MatrixMult(
                    yawMatrix,
                    pitchMatrix
                ), 
                rollMatrix
            )
        );

        getBoid(i)->dir[0]=world[2][0];
        getBoid(i)->dir[1]=world[2][1];
        getBoid(i)->dir[2]=world[2][2];

        getBoid(i)->loc+=getBoid(i)->dir*getBoid(i)->speed;
    }
}

SbMatrix BoidFlock::MatrixMult(const SbMatrix &a, const SbMatrix &b)
{
    SbMatrix ret=SbMatrix(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);

    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            for (int k=0; k<4; k++) {
                ret[i][j] += a[k][j] * b[i][k];
            }
        }
    }
    return ret;
}
