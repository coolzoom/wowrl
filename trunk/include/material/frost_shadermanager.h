/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          ShaderManager header          */
/*                                        */
/*                                        */


#ifndef FROST_SHADERMANAGER_H
#define FROST_SHADERMANAGER_H

#include "frost.h"
#include "utils/frost_utils_manager.h"

namespace Frost
{
    /// Manages vertex and pixel shaders
    class ShaderManager : public Manager<ShaderManager>
    {
    friend class Manager<ShaderManager>;
    public :

        /// Creates a new vertex shader.
        /** \param sName The name of the shader
        *   \param sFile The shader's file
        *   \note Don't put any extension in sFile, just the name of the file.
        *         The shader will then use either the .glsl or the .hlsl version
        *         depending on the chosen renderer.<br>
        *         Once your shader is created and you've set all the parameters
        *         you needed, you must call shader->Load() for it to compile.
        */
        s_ptr<VertexShader> CreateVertexShader(const s_str& sName, const s_str& sFile);

        /// Creates a new pixel shader.
        /** \param sName The name of the shader
        *   \param sFile The shader's file
        *   \note Don't put any extension in sFile, just the name of the file.
        *         The shader will then use either the .glsl or the .hlsl version
        *         depending on the chosen renderer.<br>
        *         Once your shader is created and you've set all the parameters
        *         you needed, you must call shader->Load() for it to compile.
        */
        s_ptr<PixelShader>  CreatePixelShader(const s_str& sName, const s_str& sFile);

        /// Returns a vertex shader.
        /** \param sName The name of the vertex shader
        *   \return The associated vertex shader
        *   \note Returns NULL if it doesn't exist.
        */
        s_ptr<VertexShader> GetVertexShader(const s_str& sName) const;

        /// Returns a pixel shader.
        /** \param sName The name of the pixel shader
        *   \return The associated pixel shader
        *   \note Returns NULL if it doesn't exist.
        */
        s_ptr<PixelShader>  GetPixelShader(const s_str& sName) const;

        /// Reads the content of the Shaders folder.
        void                LoadShaders();

        /// Sends shaders parameters.
        void                UpdateShaders();

        static const s_str CLASS_NAME;

    protected :

        /// Default constructor.
        /** \note This is a singleton class. That means that there can
        *         only be a single instance at the same time. That's why
        *         the constructor is not in public scope.<br>
        *         If you need to get a pointer to that unique instance,
        *         call ShaderManager::GetSingleton().<br>
        *         Note that, if GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when you close
        *         your program by calling ShaderManager::Delete() (this is
        *         automatically done by Engine).
        */
        ShaderManager();

        /// Destructor.
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete() (this is
        *         automatically done by Engine).
        */
        ~ShaderManager();

        /// Copy constructor.
        ShaderManager(const ShaderManager& mMgr);

        /// Assignment operator.
        ShaderManager& operator = (const ShaderManager& mMgr);

    private :

        s_bool ParseXMLFile_(const s_str& sFile);
        s_bool ParseVertexShader_(s_ptr<XML::Block> pShaderBlock);
        s_bool ParsePixelShader_(s_ptr<XML::Block> pShaderBlock);
        s_bool ParsePreProcessor_(s_ptr<XML::Block> pShaderBlock, s_ptr<Shader> pShader);
        s_bool ParseParams_(s_ptr<XML::Block> pShaderBlock, s_ptr<Shader> pShader);

        s_map< s_str, s_ptr<Shader> > lShaderList_;

    };
}

#endif
