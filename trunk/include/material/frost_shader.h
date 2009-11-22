/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Shader header              */
/*                                        */
/*                                        */


#ifndef FROST_SHADER_H
#define FROST_SHADER_H

#include "frost.h"

#include <OgreUnifiedHighLevelGpuProgram.h>

namespace Frost
{
    /// Handles Material special effects
    class Shader
    {
    public :

        /// Contains informations about a shader parameter
        struct AutoParam
        {
            AutoParam(const s_str& sName_, Ogre::GpuProgramParameters::AutoConstantType mType_, const s_uint& uiInfo_) :
                sName(sName_), mType(mType_), uiInfo(uiInfo_)
            {
            }

            s_str sName;
            Ogre::GpuProgramParameters::AutoConstantType mType;
            s_uint uiInfo;
        };

        /// Contains informations about a shader parameter
        struct Param
        {
            Param(const s_str& sName_, const s_var& vValue_) :
                sName(sName_), vValue(vValue_)
            {
            }

            s_str sName;
            s_var vValue;
        };


        /// Constructor.
        /** \param sName The name of this shader
        */
        Shader(const s_str& sName);

        /// Destructor.
        virtual ~Shader();

        /// Sets this shader's file.
        /** \param sFile The file
        *   \note Don't put any extension here : just the name of the file.
        *         The shader will then use either the .glsl or the .hlsl version
        *         depending on the chosen renderer.
        */
        void SetFile(const s_str& sFile);

        /// Adds a preprocessor command to this shader.
        /** \param sPPCommand The preprocessor command
        *   \note You can add several commands at the same time
        *         by separating them by commas ','.
        */
        void AddPreProcessor(const s_str& sPPCommand);

        /// Adds a fixed parameter to this shader.
        /** \param sName  The name of the shader parameter
        *   \param vValue The value to assign it
        */
        void AddParam(const s_str& sName, const s_var& vValue);

        /// Adds an automatic parameter.
        /** \param mAuto The automatic parameter
        *   \note Automatic parameters are handled by Ogre. It automatically
        *         (hence the name) sets the value depending on the rendered
        *         object (nearby lights, animation matrices, ...).
        */
        void AddAutoParam(const AutoParam& mAuto);

        /// Adds an automatic parameter.
        /** \param sName  The name of the shader parameter
        *   \param mType  What kind of value to put in this parameter
        *   \param uiInfo Optionnal parameter (number of light, texture id, ...)
        *   \note Automatic parameters are handled by Ogre. It automatically
        *         (hence the name) sets the value depending on the rendered
        *         object (nearby lights, animation matrices, ...).
        */
        void AddAutoParam(const s_str& sName, Ogre::GpuProgramParameters::AutoConstantType mType, const s_uint& uiInfo = 0);

                /// Adds an automatic parameter.
        /** \param sName  The name of the shader parameter
        *   \param sType  What kind of value to put in this parameter
        *   \param uiInfo Optionnal parameter (number of light, texture id, ...)
        *   \note Automatic parameters are handled by Ogre. It automatically
        *         (hence the name) sets the value depending on the rendered
        *         object (nearby lights, animation matrices, ...).
        */
        void AddAutoParam(const s_str& sName, const s_str& sType, const s_uint& uiInfo = 0);

        /// Adds an automatic parameter.
        /** \param sName The name of the shader parameter
        *   \note This version deduces the value type from the name.
        *         See its definition for the list of supported values.
        */
        void AddAutoParam(const s_str& sName);

        /// Adds lights automatic parameters.
        /** \param uiLightNbr   The maximum number of light sent to the shader
        *   \param bDirectional 'true' to include one directional light
        *   \note For each light, Ogre will send its position, its attenuation
        *         factors and the color.
        */
        void AddLightParams(const s_uint& uiLightNbr, const s_bool& bDirectional);

        /// Sets the value of a parameter.
        /** \param sName  The name of the parameter
        *   \param vValue The value to assign
        *   \param pPass  The pass for which to set the parameter
        *   \note If pPass is ommited, the value is assigned in all binded passes.
        */
        void SetParameter(const s_str& sName, const s_var& vValue, s_ptr<Ogre::Pass> pPass = nullptr);

        /// Checks if this shader has loaded fine.
        /** \return 'true' if everything went fine
        */
        const s_bool& IsValid() const;

        /// Compiles this shader.
        virtual void Load() = 0;

        /// Binds this shader to an Ogre::Pass.
        /** \param pPass The pass to bind
        */
        virtual void BindTo(s_ptr<Ogre::Pass> pPass) = 0;

        /// Removes this shader from an Ogre::Pass.
        /** \param pPass The pass to unbind
        */
        void UnBind(s_ptr<Ogre::Pass> pPass);

        /// Updates some of this shaders parameters.
        virtual void Update();

        /// Returns this shader's internal name.
        /** \return This shader's internal name
        *   \note When you create a shader with the ShaderManager, you provide
        *         it a name. Internally, the manager adds "_VS" or "_PS" if
        *         your shader is a vertex or a pixel shader.
        */
        const s_str& GetOgreName();

        static const s_str CLASS_NAME;

    protected :

        void WriteParams_(s_ptr<Ogre::Pass> pPass);
        virtual Ogre::GpuProgramParametersSharedPtr GetOgreParamList_(s_ptr<Ogre::Pass> pPass) = 0;

        s_str sName_;
        s_str sFile_;
        s_bool bIsValid_;
        s_bool bIsLoaded_;
        s_str sPPCommands_;
        s_ptr<Ogre::UnifiedHighLevelGpuProgram> pOgreShader_;
        s_ctnr<AutoParam> lAutoParamList_;
        s_ctnr<Param>     lParamList_;

        s_ctnr< s_ptr<Ogre::Pass> > lBindedPassList_;

        s_bool bSendSunParameters_;
    };

    /// Controls vertices in the graphics card
    class VertexShader : public Shader
    {
    public :

        /// Constructor.
        /** \param sName The name of this shader
        */
        VertexShader(const s_str& sName);

        /// Destructor.
        ~VertexShader();

        /// Tells Ogre this vertex shader does skeletal animation.
        /** \note Ogre does software skeletal animation. As your shader
        *         implements it in hardware, you need to tell Ogre that
        *         you take care of this by yourself.
        */
        void NotifyUsesSkeletalAnimation();

        /// Compiles this shader.
        void Load();

        /// Binds this shader to an Ogre::Pass.
        /** \param pPass The pass to bind
        */
        void BindTo(s_ptr<Ogre::Pass> pPass);

        static const s_str CLASS_NAME;

    private :

        Ogre::GpuProgramParametersSharedPtr GetOgreParamList_(s_ptr<Ogre::Pass> pPass);

        s_bool bSkeletalAnim_;

    };

    /// Controls pixels in the graphics card
    class PixelShader : public Shader
    {
    public :

        /// Contains informations about a GLSL sampler parameters
        struct SamplerParam
        {
            SamplerParam(const s_str& sName_, const s_uint& uiID_) :
                sName(sName_), uiID(uiID_)
            {
            }

            s_str sName;
            s_uint uiID;
        };

        /// Constructor.
        /** \param sName The name of this shader
        */
        PixelShader(const s_str& sName);

        /// Destructor.
        ~PixelShader();

        /// Binds a sampler parameter to a texture unit.
        /** \param sName The name of the sampler
        *   \param uiID  The id of the texture unit
        *   \note This is only necessary for GLSL.
        */
        void BindTextureSampler(const s_str& sName, const s_uint& uiID);

        /// Compiles this shader.
        void Load();

        /// Binds this shader to an Ogre::Pass.
        /** \param pPass The pass to bind
        */
        void BindTo(s_ptr<Ogre::Pass> pPass);

        static const s_str CLASS_NAME;

    private :

        Ogre::GpuProgramParametersSharedPtr GetOgreParamList_(s_ptr<Ogre::Pass> pPass);

        s_ctnr<SamplerParam> lSamplerParamList_;

    };
}

#endif
