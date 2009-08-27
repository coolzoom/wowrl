// Note : inspired from Ogre's example shaders

// Input (names are forced by Ogre)
attribute vec4 vertex;
attribute vec3 normal;
attribute vec4 blendWeights;
attribute vec4 blendIndices;
attribute vec4 uv0;

// Output
varying vec3  vBlendedNormal;
varying vec3  vBlendedPosition;
#ifdef MOTION_BLUR
    varying float vDepth;
#endif

// Provided by Ogre
uniform mat4  mViewProj;     // viewproj_matrix
uniform vec4  mBoneMat[180]; // world_matrix_array_3x4
#ifdef MOTION_BLUR
    uniform vec4  mCamPos;
    uniform float mCamMaxDepth;
#endif

void main()
{
    vBlendedPosition = vec3(0, 0, 0);
    vBlendedNormal = vec3(0, 0, 0);
    
    // Temporary variables
    mat4  tBlendVMatrix; tBlendVMatrix[3] = vec4(0);
    mat3  tBlendNMatrix;
    int   tBoneIndex;
    float tWeight;
    
    for (int i = 0; i < 4; ++i)
    {
        tBoneIndex = int(blendIndices[i])*3;
        tWeight = blendWeights[i];
        
        // Apply bones to the vertex
        tBlendVMatrix[0] = mBoneMat[tBoneIndex + 0];
        tBlendVMatrix[1] = mBoneMat[tBoneIndex + 1];
        tBlendVMatrix[2] = mBoneMat[tBoneIndex + 2];
        vBlendedPosition += tWeight * (vertex * tBlendVMatrix).xyz;

        // Apply bones to the normal
        tBlendNMatrix = mat3(tBlendVMatrix[0].xyz, tBlendVMatrix[1].xyz, tBlendVMatrix[2].xyz);
		vBlendedNormal += tWeight * (normal * tBlendNMatrix);
    }
    
    // Apply position and camera projection
    gl_Position = mViewProj * vec4(vBlendedPosition, 1);
    #ifdef MOTION_BLUR
        vDepth = distance(vBlendedPosition, mCamPos.xyz)/mCamMaxDepth;
    #endif

    // Do not change texture coordinates
    gl_TexCoord[0] = uv0;
}
