/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               XXX header               */
/*                                        */
/*                                        */


#ifndef FROST_XML_DOCUMENT_H
#define FROST_XML_DOCUMENT_H

#include "frost.h"
#include "frost_xml_block.h"

namespace Frost
{
namespace XML
{
    /// Parses an XML file
    /** Uses a definition file (*.def) to validate
    *   the parsed file.
    */
    class Document
    {
    public :

        /// Default constructor.
        /** \param sFileName    The path to the file you want to parse
        *   \param sDefFileName The path to the definition file to use
        *   \note The definition file is parsed in this constructor.
        */
        Document(const s_str& sFileName, const s_str& sDefFileName);

        /// Destructor.
        ~Document();

        /// Returns this file's main block.
        /** \return This file's main block
        */
        s_ptr<Block>  GetMainBlock();

        /// Returns this file's name.
        /** \return This file's name
        *   \note Only used internaly.
        */
        const s_str&  GetFileName() const;

        /// Returns the line that is being parsed.
        /** \return The line that is being parsed
        *   \note Only used internaly.<br>If you call this
        *         method before Check(), it will return the
        *         total number of line in the definition file.
        *         After, it will return the total number of
        *         line in the XML file.
        */
        const s_uint& GetLineNbr() const;

        /// Parses and validates the XML file.
        /** \return 'true' if everything went fine
        *   \note This function takes care of retreiving
        *         values from the file and also checks the
        *         content of the file acording to the *.def
        *         file you provided.
        */
        s_bool        Check();

        /// Returns a predefined Block.
        /** \param sName The name of the predefined Block you want
        *   \note Only used in loading stage.
        */
        s_ptr<Block>  GetPredefinedBlock(const s_str& sName);

        static const s_str CLASS_NAME;

    private :

        s_bool CheckLineSynthax_(s_str& sLine);
        s_bool LoadDefinition_();
        s_bool ReadPreDefCommands_(s_str& sName, s_str& sParent, s_uint& uiMin, s_uint& uiMax, s_bool& bPreDefining, s_bool& bLoad, s_bool& bRadio, const s_bool& bMultiline, s_ptr<Block> pParent);
        s_bool ParseArguments_(s_ptr<Block> pActual, const s_ctnr<s_str>& lAttribs);

        s_str  sFileName_;
        s_str  sDefFileName_;
        s_str  sActualFileName_;
        s_uint uiLineNbr_;
        s_bool bValid_;
        Block  mMainBlock_;

        std::map<s_str, Block> lPredefinedBlockList_;
    };
}
}

#endif
