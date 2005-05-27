// FaceModel.cpp : Defines the class behaviors for the face model
//

#include "FaceModel.h"

CFaceModel::CFaceModel(){

    m_initialized=false;
}

CFaceModel::~CFaceModel(){
}

bool CFaceModel::Init(char *path){

    m_min[0]=m_min[1]=m_min[2]=m_max[0]=m_max[1]=m_max[2]=0.0f;

    char fileName[_MAX_PATH];

    sprintf(fileName,"%s\\%s",path,FILENAME_VERTEX);
    // *** load topology data from file
    if (!LoadVertices(fileName)){   // *** vertices
        ErrorMessage(ERR_INIT_FACEMODEL_VERTEX);
        return false;
    }
    
    sprintf(fileName,"%s\\%s",path,FILENAME_TRIANGLE);
    if (!LoadTriangles(fileName)){  // *** links
        ErrorMessage(ERR_INIT_FACEMODEL_TRIANGLE);
        return false;
    }

    // *** build smooth normal information
    for (int m=0;m<MAX_FACE_TRIANGLE;m++){
        for (int n=0;n<3;n++){
            m_normalVectorList[m_triangle[m].GetVertexIndex(n)].addtail(m);
        }
    }
    for (int n=0;n<MAX_FACE_VERTEX;n++){
        m_smoothNormalEnabled[n]=true;
    }
    for (int p=0;p<sizeof(disableNormalIndex)/sizeof(int);p++){
        m_smoothNormalEnabled[disableNormalIndex[p]]=false;
    }

    // *** init eye parameters
    // WARNING: constants are highly model specific!
    m_leftEyeCenter=(m_vertex[VERTEX_ID_EYE_TOP_LEFT]+
        m_vertex[VERTEX_ID_EYE_BOTTOM_LEFT])/2.0f;
    m_rightEyeCenter=(m_vertex[VERTEX_ID_EYE_TOP_RIGHT]+
        m_vertex[VERTEX_ID_EYE_BOTTOM_RIGHT])/2.0f;

    // *** conformation params not loaded yet
    m_conformationValid=false;
    m_glasses=false;
    m_transparency=0;

    // *** if not returned by this, the face is initialized without problems
    m_initialized=true;

    return true;
}

void CFaceModel::DrawTest(float zoom){

    // define the vertices of the cube
    GLfloat vertex[8][3]={
        { 0.5f, 0.5f, 0.5f}, // 0
        {-0.5f, 0.5f, 0.5f}, // 1
        {-0.5f,-0.5f, 0.5f}, // 2
        { 0.5f,-0.5f, 0.5f}, // 3
        {-0.5f, 0.5f,-0.5f}, // 4
        { 0.5f, 0.5f,-0.5f}, // 5
        { 0.5f,-0.5f,-0.5f}, // 6
        {-0.5f,-0.5f,-0.5f}  // 7       
    };
    int vertex_order[6][4]={
        {0,1,2,3},
        {5,0,3,6},
        {4,5,6,7},
        {1,4,7,2},
        {4,1,0,5},
        {7,6,3,2}
    };
    GLfloat color[6][3]={
        { 1.0f, 0.0f, 0.0f}, // red
        { 0.0f, 1.0f, 0.0f}, // green
        { 0.0f, 0.0f, 1.0f}, // blue
        { 1.0f, 1.0f, 0.0f}, // yellow
        { 1.0f, 0.0f, 1.0f}, // purple
        { 0.0f, 1.0f, 1.0f}  // cyan
    };
    GLfloat normal[6][3]={
        { 0.0f, 0.0f, 1.0f},
        { 1.0f, 0.0f, 0.0f},
        { 0.0f, 0.0f,-1.0f},
        {-1.0f, 0.0f, 0.0f},
        { 0.0f, 1.0f, 0.0f},
        { 0.0f,-1.0f, 0.0f}     
    };

    glBegin(GL_QUADS);
    for (int i=0;i<6;i++){
        glNormal3fv(normal[i]);
        glColor3fv(color[i]);
        for (int j=0;j<4;j++){                  
            int n0=vertex_order[i][j];
            glVertex3f(
                vertex[n0][0]*zoom,
                vertex[n0][1]*zoom,
                vertex[n0][2]*zoom);
        }
    }
    glEnd();
}

bool CFaceModel::LoadVertices(char *fileName){

    // *** temporary variables for reading
    char line[128];
    // *** we use the 'serial' parameter just for reading in data, we won't store it!
    int serial,fid; 
    float x,y,z;

    // *** open vertex file
    FILE *stream=fopen(fileName,"r");
    if (!stream) return false;
    
    int i=0;
    while (i<MAX_VERTEX && fgets(line,256,stream)){
        if (line[0]!='#'){      
            sscanf(line,"%d %d %f %f %f",&serial,&fid,&x,&y,&z);
    
            // *** serial = i so it's not worth storing
            m_vertex[i].Set(x,y,z);
            m_vertexIndex[i]=fid;
            //pnt->SetTextureMap(x*ZOOM_RATE+DRAW_CENTER_X,-y*ZOOM_RATE+DRAW_CENTER_Y);
            if (x<m_min[0]) m_min[0]=x;
            if (y<m_min[1]) m_min[1]=y;
            if (z<m_min[2]) m_min[2]=z;
            if (x>m_max[0]) m_max[0]=x;
            if (y>m_max[1]) m_max[1]=y;
            if (z>m_max[2]) m_max[2]=z;
            i++;
        }
    }
    fclose(stream);

    return true;
}

bool CFaceModel::LoadTriangles(char *fileName){
    
    // *** temporary variables for reading
    char line[128];
    // *** we use the 'serial' parameter just for reading in data, we won't store it!
    int serial,fid,a,b,c; 

    // *** open vertex file
    FILE *stream=fopen(fileName,"r");
    if (!stream) return false;
    
    int i=0;
    while (i<MAX_TRIANGLE && fgets(line,256,stream)){
        if (line[0]!='#'){      
            // serial = i so it's not worth storing
            sscanf(line,"%d %d %d %d %d",&serial,&fid,&a,&b,&c);
            m_triangle[i].Set(a,b,c);
            m_triangleIndex[i]=fid;
            i++;
        }
    }
    fclose(stream);

    return true;
}

void CFaceModel::GetMinMax(float *min,float *max){

    for (int i=0;i<3;i++){
        min[i]=m_min[i];
        max[i]=m_max[i];
    }
}

void CFaceModel::SetConformation(CVertex *vertex,bool glasses,int transparency){

    for (int i=0;i<MAX_VERTEX;m_conformation[i]=vertex[i],i++);
    m_conformationValid=true;
    m_glasses=glasses;
    m_transparency=transparency;
}

// *** defines the normal vector of a triangle facet in OpenGL
CVertex CFaceModel::NormalVector(CTriangle *triangle){
    
    float v[3][3];
    float v1[3],v2[3];
    static const int x = 0;
    static const int y = 1;
    static const int z = 2;
    GLfloat out[3];

    // initialize vector 'v'
    for (int i=0;i<3;i++){
        for (int j=0;j<3;j++){
            switch (j){
                case 0:
                    v[i][j]=m_workVertex[triangle->GetVertexIndex(i)].X();
                    break;
                case 1:
                    v[i][j]=m_workVertex[triangle->GetVertexIndex(i)].Y();
                    break;
                case 2:
                    v[i][j]=m_workVertex[triangle->GetVertexIndex(i)].Z();
                    break;
            }
        }
    }

    // calculate two outs from the three points
    v1[x] = v[0][x] - v[1][x];
    v1[y] = v[0][y] - v[1][y];
    v1[z] = v[0][z] - v[1][z];

    v2[x] = v[1][x] - v[2][x];
    v2[y] = v[1][y] - v[2][y];
    v2[z] = v[1][z] - v[2][z];

    // take the cross product of the two outs to get
    // the normal out which will be stored in out
    out[x] = v1[y]*v2[z] - v1[z]*v2[y];
    out[y] = v1[z]*v2[x] - v1[x]*v2[z];
    out[z] = v1[x]*v2[y] - v1[y]*v2[x];

    // normalize the out (shorten length to one)    
    float length = (float) sqrt((out[0]*out[0])+(out[1]*out[1])+(out[2]*out[2]));

    return CVertex(out[0],out[1],out[2]);
}

float* CFaceModel::GetColors(float *color){

    float *result=new float[MAX_FID*3];
    int colorNum;
    bool blend;
    float alpha=0.3f;

    for (int j=0;j<MAX_FID;j++){
        switch (j){

            case FID_EYEAREA_LEFT:
            case FID_EYEAREA_RIGHT:
            case FID_FOREHEAD_LEFT:
            case FID_FOREHEAD_RIGHT:
            case FID_NECK_LEFT:
            case FID_NECK_RIGHT:
                colorNum=COL_FACE;
                blend=false;
                break;

            case FID_LIP_LEFT:
            case FID_LIP_RIGHT:
                colorNum=COL_LIP;
                blend=false;
                break;

            case FID_CHEEK_LEFT:
            case FID_CHEEK_RIGHT:
                colorNum=COL_CHEEK;
                blend=true;
                break;

            case FID_NOSE:
                colorNum=COL_NOSE;
                blend=true;
                break;

            case FID_EYELID_LEFT:
            case FID_EYELID_RIGHT:
                colorNum=COL_EYELID;
                blend=true;
                break;

            case FID_EYEBROW_LEFT:
            case FID_EYEBROW_RIGHT:
                colorNum=COL_EYEBROW;
                blend=false;
                break;

            case FID_MOUTHAREA_LEFT:
            case FID_MOUTHAREA_RIGHT:
                colorNum=COL_MOUSTACHE;
                blend=true;
                break;          

            case FID_CHIN_LEFT:
            case FID_CHIN_RIGHT:
                colorNum=COL_CHIN;
                blend=true;
                break;

            case FID_IRIS_LEFT:
            case FID_IRIS_RIGHT:
                colorNum=COL_IRIS;
                blend=false;
                break;

            case FID_EYEBALL_LEFT:
            case FID_EYEBALL_RIGHT:
                colorNum=COL_EYEBALL;
                blend=false;
                break;

            case FID_TEETH_UPPER:
            case FID_TEETH_LOWER:
                colorNum=COL_TEETH;
                blend=false;
                break;

            case FID_GUM_UPPER:
            case FID_GUM_LOWER:
                colorNum=COL_GUM;
                blend=false;
                break;

            case FID_HAIR_LEFT:
            case FID_HAIR_RIGHT:
                colorNum=COL_HAIR;
                blend=false;
                break;

            case FID_EYELASH_LEFT:
            case FID_EYELASH_RIGHT:
                colorNum=COL_EYELASH;
                blend=false;
                break;

            case FID_PUPIL_LEFT:
            case FID_PUPIL_RIGHT:
                colorNum=COL_PUPIL;
                blend=false;
                break;

            case FID_EAR_LEFT:
            case FID_EAR_RIGHT:
                colorNum=COL_EAR;
                blend=true;
                break;
            
            case FID_TONGUE:
                colorNum=COL_TONGUE;
                blend=false;
                break;

            case FID_GLASSES:
                colorNum=COL_GLASSES;
                blend=false;
                break;
        }
        if (blend){         
            result[j*3]=
                ( color[colorNum*3]*alpha + color[COL_FACE*3]*(1.0f-alpha) )/255.0f;
            result[j*3+1]=
                ( color[colorNum*3+1]*alpha + color[COL_FACE*3+1]*(1.0f-alpha) )/255.0f;
            result[j*3+2]=
                ( color[colorNum*3+2]*alpha + color[COL_FACE*3+2]*(1.0f-alpha) )/255.0f;
        }
        else {
            result[j*3]  =color[colorNum*3]/255.0f;
            result[j*3+1]=color[colorNum*3+1]/255.0f;
            result[j*3+2]=color[colorNum*3+2]/255.0f;
        }
    }

    return result;
}

bool CFaceModel::Draw(CVertex* deform,float *transform,float *color,
                      bool smoothEnabled,bool coloringEnabled){

    float *fid_colors=new float[MAX_FID*3];
    fid_colors=GetColors(color);

    if (!m_initialized){ 
        DrawTest(transform[2]);
    }
    else {
        // *** initialize work vertex variable with initial vertex data
        for (int i=0;i<MAX_VERTEX;i++){
            m_workVertex[i]=m_conformationValid ? 
                m_conformation[i]+deform[i] : m_vertex[i]+deform[i];
        }

        // *** conformation control for each part of the face
        // ...

        // *** render each part of the face
        DrawFace(transform,fid_colors,smoothEnabled,coloringEnabled);

        // *** render accessories (e.g. glasses, jewellery)
        if (m_glasses) DrawAccessories(transform,fid_colors,coloringEnabled);
    }

    return true;
}

// *** render auxiliary objects (eyelashes, pupil, eye glow)
// *** WARNING: constants are highly model specific!
void CFaceModel::DrawAuxiliary(float *transform,float *color,bool coloringEnabled){

    // *** left eye 
    glPushMatrix();
    glTranslatef(
        m_leftEyeCenter.X()*transform[2],
        m_leftEyeCenter.Y()*transform[2],
        m_leftEyeCenter.Z()*transform[2]);
    glRotatef(transform[3],1.0f,0.0f,0.0f);
    glRotatef(transform[4],0.0f,1.0f,0.0f); 

    // *** left eye glow
    glPointSize(1.3f*transform[2]*transform[7]);
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_POINTS);     
        glNormal3f(0.0f,0.0f,1.0f);
        glVertex3f(
            (m_workVertex[VERTEX_ID_PUPIL_LEFT].X()-m_leftEyeCenter.X())*transform[2]+0.02f,
            (m_workVertex[VERTEX_ID_PUPIL_LEFT].Y()-m_leftEyeCenter.Y())*transform[2]+0.02f,
            (m_workVertex[VERTEX_ID_PUPIL_LEFT].Z()-m_leftEyeCenter.Z())*transform[2]+0.002f
        );
    glEnd();

    // *** left pupil
    glPointSize(4.0f*transform[2]*transform[7]);
    glColor3f(
        color[FID_PUPIL_LEFT*3],
        color[FID_PUPIL_LEFT*3+1],
        color[FID_PUPIL_LEFT*3+2]
    );
    glBegin(GL_POINTS);
        glNormal3f(0.0f,0.0f,0.0f);
        glVertex3f(
            (m_workVertex[VERTEX_ID_PUPIL_LEFT].X()-m_leftEyeCenter.X())*transform[2],
            (m_workVertex[VERTEX_ID_PUPIL_LEFT].Y()-m_leftEyeCenter.Y())*transform[2],
            (m_workVertex[VERTEX_ID_PUPIL_LEFT].Z()-m_leftEyeCenter.Z())*transform[2]+0.001f
        );
    glEnd();

    glPopMatrix();

    // *** right eye    
    glPushMatrix();
    glTranslatef(
        m_rightEyeCenter.X()*transform[2],
        m_rightEyeCenter.Y()*transform[2],
        m_rightEyeCenter.Z()*transform[2]);
    glRotatef(transform[5],1.0f,0.0f,0.0f);
    glRotatef(transform[6],0.0f,1.0f,0.0f); 

    // *** rigth eye glow
    glPointSize(1.3f*transform[2]*transform[7]);
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_POINTS);     
        glNormal3f(0.0f,0.0f,1.0f);
        glVertex3f(
            (m_workVertex[VERTEX_ID_PUPIL_RIGHT].X()-m_rightEyeCenter.X())*transform[2]+0.02f,
            (m_workVertex[VERTEX_ID_PUPIL_RIGHT].Y()-m_rightEyeCenter.Y())*transform[2]+0.02f,
            (m_workVertex[VERTEX_ID_PUPIL_RIGHT].Z()-m_rightEyeCenter.Z())*transform[2]+0.002f
        );
    glEnd();

    // *** rigth pupil
    glPointSize(4.0f*transform[2]*transform[7]);
    glColor3f(
        color[FID_PUPIL_RIGHT*3],
        color[FID_PUPIL_RIGHT*3+1],
        color[FID_PUPIL_RIGHT*3+2]
    );
    glBegin(GL_POINTS);
        glNormal3f(0.0f,0.0f,0.0f);
        glVertex3f(
            (m_workVertex[VERTEX_ID_PUPIL_RIGHT].X()-m_rightEyeCenter.X())*transform[2],
            (m_workVertex[VERTEX_ID_PUPIL_RIGHT].Y()-m_rightEyeCenter.Y())*transform[2],
            (m_workVertex[VERTEX_ID_PUPIL_RIGHT].Z()-m_rightEyeCenter.Z())*transform[2]+0.001f
        );
    glEnd();

    glPopMatrix();
    
    // *** eyelashes
    glColor3f(
        color[FID_EYELASH_LEFT*3],
        color[FID_EYELASH_LEFT*3+1],
        color[FID_EYELASH_LEFT*3+2]
    );
    glNormal3f(0.0f,0.0f,0.0f);
    glLineWidth(2.0f*transform[2]*transform[7]);
    glBegin(GL_LINE_STRIP);
        for (int k=VERTEX_ID_EYELASH_LEFT1;k<=VERTEX_ID_EYELASH_LEFT2;k++) 
            glVertex3f(
                m_workVertex[k].X()*transform[2],
                m_workVertex[k].Y()*transform[2],
                m_workVertex[k].Z()*transform[2]
        );
    glEnd();
    glBegin(GL_LINE_STRIP);
        for (int l=VERTEX_ID_EYELASH_RIGHT1;l<=VERTEX_ID_EYELASH_RIGHT2;l++) 
            glVertex3f(
                m_workVertex[l].X()*transform[2],
                m_workVertex[l].Y()*transform[2],
                m_workVertex[l].Z()*transform[2]
        );
    glEnd();
}

// *** render accessories - e.g. glasses, jewellery
void CFaceModel::DrawAccessories(float *transform,float *color,bool coloringEnabled){

    int vertexIndex;

    // *** draw the frame of the glasses
    int frameVertex[5][18]={
        {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,-1},
        {16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,16,-1},
        {4,32,33,20,-1},
        {13,34,35,-1},
        {29,36,37,-1}
    };
    int j;

    glColor3f(0.1f,0.1f,0.1f);
    glLineWidth(transform[2]*transform[7]);
    glNormal3f(0.0f,0.0f,0.0f);

    for (int i=0;i<5;i++){  
    
        j=0;
        glBegin(GL_LINE_STRIP);
        while (frameVertex[i][j]!=-1){
            glVertex3f(
                m_workVertex[VERTEX_ID_GLASSES+frameVertex[i][j]].X()*transform[2],
                m_workVertex[VERTEX_ID_GLASSES+frameVertex[i][j]].Y()*transform[2],
                m_workVertex[VERTEX_ID_GLASSES+frameVertex[i][j]].Z()*transform[2]
            );
            j++;
        }
        glEnd();
    }

    // *** draw the lenses of the glasses
    if (coloringEnabled){

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);       
            glColor4f(
                color[FID_GLASSES*3],
                color[FID_GLASSES*3+1],
                color[FID_GLASSES*3+2],
                (float)m_transparency/100.0f
        );
        float x,y,z,sum;
        glBegin(GL_TRIANGLES);
        for (int i=MAX_FACE_TRIANGLE;i<MAX_TRIANGLE;i++){
            if (i<MAX_FACE_TRIANGLE+14){
                x=0.01f*(i-MAX_FACE_TRIANGLE-7);
                y=0.01f*(i-MAX_FACE_TRIANGLE-7)+0.1f;
            }
            else {
                x=0.01f*(i-MAX_FACE_TRIANGLE-14);
                y=0.01f*(i-MAX_FACE_TRIANGLE-14)+0.1f;
            }
            z=1.0f;
            sum=(float)sqrt(x*x+y*y+z*z);
            glNormal3f(x/sum,y/sum,z/sum);
            for (int j=0;j<3;j++){              
                vertexIndex=m_triangle[i].GetVertexIndex(j);
                glVertex3f(
                    m_workVertex[vertexIndex].X()*transform[2],
                    m_workVertex[vertexIndex].Y()*transform[2],
                    m_workVertex[vertexIndex].Z()*transform[2]
                );
            }
        }
        glEnd();
    }
}

// *** render face
void CFaceModel::DrawFace(float *transform,float *color,
                          bool smoothEnabled,bool coloringEnabled){

    CVertex defaultNormal;
    int vertexIndex;
    
    // *** set line width for wireframe because the auxiliary eyelash will change this parameter    
    glLineWidth(1.5f*transform[2]*transform[7]);

    // *** if face coloring is not enabled, set WHITE color for all the face here
    if (!coloringEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    }

    // *** head rotation (in addition to general user rotation set by mouse)
    glRotatef(transform[0],1.0f,0.0f,0.0f);
    glRotatef(transform[1],0.0f,1.0f,0.0f);

    for (int i=0;i<MAX_FACE_TRIANGLE;i++){

        defaultNormal=NormalVector(&m_triangle[i]);
        defaultNormal.Normalize();

        // *** faceted like normal vector if smoothing is disabled for the whole triangle
        if (!smoothEnabled) glNormal3f(defaultNormal.X(),defaultNormal.Y(),defaultNormal.Z());

        // *** "pre vertex render" steps
        switch (i){
                
            case 0:
                glBegin(GL_TRIANGLES);
                break;

            case TRIANGLE_ID_IRIS_LEFT:
                glEnd();
                glPushMatrix();
                glTranslatef(
                    m_leftEyeCenter.X()*transform[2],
                    m_leftEyeCenter.Y()*transform[2],
                    m_leftEyeCenter.Z()*transform[2]);
                glRotatef(transform[3],1.0f,0.0f,0.0f);
                glRotatef(transform[4],0.0f,1.0f,0.0f);                 
                glBegin(GL_TRIANGLES);
                break;

            case TRIANGLE_ID_IRIS_RIGHT:
                glEnd();
                glPushMatrix();
                glTranslatef(
                    m_rightEyeCenter.X()*transform[2],
                    m_rightEyeCenter.Y()*transform[2],
                    m_rightEyeCenter.Z()*transform[2]);
                glRotatef(transform[5],1.0f,0.0f,0.0f);
                glRotatef(transform[6],0.0f,1.0f,0.0f);                 
                glBegin(GL_TRIANGLES);
                break;          
        }

        if (coloringEnabled){
            
            switch (m_triangleIndex[i]){
                
                case FID_CHEEK_LEFT:
                case FID_CHEEK_RIGHT:
                case FID_CHIN_LEFT:
                case FID_CHIN_RIGHT:
                case FID_NOSE:
                case FID_NECK_LEFT:
                case FID_NECK_RIGHT:
                    if (!smoothEnabled){
                        glColor3f(
                            color[FID_FOREHEAD_LEFT*3],
                            color[FID_FOREHEAD_LEFT*3+1],
                            color[FID_FOREHEAD_LEFT*3+2]
                        );
                    }
                    break;
                
                case FID_MOUTHAREA_LEFT:
                case FID_MOUTHAREA_RIGHT:               
                    if (!smoothEnabled){
                        glColor3f(
                            color[FID_FOREHEAD_LEFT*3],
                            color[FID_FOREHEAD_LEFT*3+1],
                            color[FID_FOREHEAD_LEFT*3+2]
                        );
                    }
                    else {
                        glColor3f(
                            color[m_triangleIndex[i]*3],
                            color[m_triangleIndex[i]*3+1],
                            color[m_triangleIndex[i]*3+2]
                        );
                    }
                    break;
                    
                default:
                    glColor3f(
                        color[m_triangleIndex[i]*3],
                        color[m_triangleIndex[i]*3+1],
                        color[m_triangleIndex[i]*3+2]
                    );
                    break;
            }
        }

        for (int j=0;j<3;j++){

            vertexIndex=m_triangle[i].GetVertexIndex(j);
            // *** drawing the triangle
            // load texture coordinates
            // ...

            // *** creating smooth normals if enabled
            if (smoothEnabled){

                if (m_smoothNormalEnabled[vertexIndex]){
                    m_normalVector[vertexIndex]=CVertex(0.0f,0.0f,0.0f);                    
                    m_normalVectorList[vertexIndex].gohead();
                    do{
                        m_normalVector[vertexIndex]+=NormalVector(
                            &m_triangle[(int)(m_normalVectorList[vertexIndex].getnodeinfo())]);
                    }
                    while (m_normalVectorList[vertexIndex].gonext());
                    m_normalVector[vertexIndex].Normalize();
                    glNormal3f(
                        m_normalVector[vertexIndex].X(),
                        m_normalVector[vertexIndex].Y(),
                        m_normalVector[vertexIndex].Z()
                    );
                }
                else glNormal3f(defaultNormal.X(),defaultNormal.Y(),defaultNormal.Z());
            }

            // *** if facial coloring is enabled, set color if current vertex
            if (coloringEnabled && smoothEnabled) {

                switch (m_triangleIndex[i]){
                
                    case FID_CHEEK_LEFT:
                    case FID_CHEEK_RIGHT:
                    case FID_CHIN_LEFT:
                    case FID_CHIN_RIGHT:
                    case FID_NOSE:
                    case FID_NECK_LEFT:
                    case FID_NECK_RIGHT:
                        glColor3f(
                            color[m_vertexIndex[vertexIndex]*3],
                            color[m_vertexIndex[vertexIndex]*3+1],
                            color[m_vertexIndex[vertexIndex]*3+2]
                        );
                        break;
                    
                    default:
                        break;
                        
                }
            }

            // *** define 3D coordinates of the current triangle vertex
            switch (m_vertexIndex[vertexIndex]){

                case FID_EYEBALL_LEFT:
                case FID_IRIS_LEFT:
                    glVertex3f(
                        (m_workVertex[vertexIndex].X()-m_leftEyeCenter.X())*transform[2],
                        (m_workVertex[vertexIndex].Y()-m_leftEyeCenter.Y())*transform[2],
                        (m_workVertex[vertexIndex].Z()-m_leftEyeCenter.Z())*transform[2]
                    );
                    break;
                    
                case FID_EYEBALL_RIGHT:
                case FID_IRIS_RIGHT:
                    glVertex3f(
                        (m_workVertex[vertexIndex].X()-m_rightEyeCenter.X())*transform[2],
                        (m_workVertex[vertexIndex].Y()-m_rightEyeCenter.Y())*transform[2],
                        (m_workVertex[vertexIndex].Z()-m_rightEyeCenter.Z())*transform[2]
                    );
                    break;

                default:
                    glVertex3f(
                        m_workVertex[vertexIndex].X()*transform[2],
                        m_workVertex[vertexIndex].Y()*transform[2],
                        m_workVertex[vertexIndex].Z()*transform[2]
                    );
                    break;
            }               
                
        } // vertices of current triangle
    
        // *** "post vertex render" steps
        switch (i){
                    
            case TRIANGLE_ID_EYE_LEFT_LAST:
            case TRIANGLE_ID_EYE_RIGHT_LAST:
                glEnd();
                glPopMatrix();
                glBegin(GL_TRIANGLES);
                break;
        }

    } // triangles

    glEnd();

    // *** draw auxiliary parts of the face
    if (coloringEnabled) DrawAuxiliary(transform,color,coloringEnabled);
}
