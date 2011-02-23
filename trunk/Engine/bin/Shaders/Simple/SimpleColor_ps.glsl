// Note : inspired from Ogre's example shaders

varying vec4 vColor;
#ifdef MOTION_BLUR
    varying vec4 vPosition;
#endif

void main()
{
    #ifdef MOTION_BLUR
        gl_FragData[0] = vColor;
        gl_FragData[1] = vec4(1,1,1,1);
        gl_FragData[1].r = vPosition.z/vPosition.w;
    #else
        gl_FragColor = vColor;
    #endif
}
