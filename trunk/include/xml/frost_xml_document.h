/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          XML::Document header          */
/*                                        */
/*                                        */


#ifndef FROST_XML_DOCUMENT_H
#define FROST_XML_DOCUMENT_H

#include <frost_utils_types.h>
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
    friend class XML::Block;
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
        const s_str&  GetCurrentFileName() const;

        /// Returns the line that is being parsed.
        /** \return The line that is being parsed
        *   \note Only used internaly.<br>If you call this
        *         method before Check(), it will return the
        *         total number of line in the definition file.
        *         After, it will return the total number of
        *         line in the XML file.
        */
        const s_uint& GetCurrentLineNbr() const;

        /// Returns the current parsing location.
        /** \return The current parsing location
        *   \note Format : filename:line
        */
        s_str         GetCurrentLocation() const;

        /// Parses and validates the XML file.
        /** \param sPreProcCommands Preprocessor commands to use when parsing
        *                           the xml file
        *   \return 'true' if everything went fine
        *   \note This function takes care of retreiving
        *         values from the file and also checks the
        *         content of the file acording to the *.def
        *         file you provided.<br>
        *         The preprocessor commands must be separated by commas ','.
        */
        s_bool        Check(const s_str& sPreProcCommands = "");

        static const s_str CLASS_NAME;

    protected :

        /// Creates a new predefined Block with inheritance.
        /** \param sName        The name of the new predefined Block
        *   \param sInheritance The name of the inherited Block
        */
        void         CreatePredefinedBlock(const s_str& sName, const s_str& sInheritance = "");

        /// Returns a predefined Block.
        /** \param sName The name of the predefined Block you want
        *   \note Only used in loading stage.
        */
        s_ptr<Block> GetPredefinedBlock(const s_str& sName);

        /// Flags this Document as invalid.
        void         SetInvalid();

    private :

        /** \cond NOT_REMOVE_FROM_DOC
        */
        class State
        {
        public :

            enum ID
            {
                STATE_DEF,
                STATE_XML
            };

            State();

            virtual ~State();

            virtual s_str ReadTagName(const s_str& sTagContent) const = 0;
            virtual void  ReadOpeningTag(const s_str& sTagContent) = 0;
            virtual void  ReadSingleTag(const s_str& sTagContent) = 0;
            virtual void  ReadEndingTag(const s_str& sTagContent) = 0;

            void          SetDocument(s_ptr<Document> pDoc);
            void          SetCurrentBlock(s_ptr<Block> pBlock);
            void          SetCurrentParentBlock(s_ptr<Block> pParentBlock);
            void          AddContent(const s_str& sContent);
            void          ResetContent();
            const ID&     GetID() const;

        protected :

            s_ptr<Document> pDoc_;
            ID mID_;

            s_ptr<Block> pCurrentBlock_;
            s_ptr<Block> pCurrentParentBlock_;
            s_str        sBlockContent_;
        };
        friend class State;

        class XMLState : public State
        {
        public :

            XMLState();

            s_str ReadTagName(const s_str& sTagContent) const;
            void  ReadOpeningTag(const s_str& sTagContent);
            void  ReadSingleTag(const s_str& sTagContent);
            void  ReadEndingTag(const s_str& sTagContent);
        };
        friend class XMLState;

        class DefState : public State
        {
        public :

            DefState();

            s_str ReadTagName(const s_str& sTagContent) const;
            void  ReadOpeningTag(const s_str& sTagContent);
            void  ReadSingleTag(const s_str& sTagContent);
            void  ReadEndingTag(const s_str& sTagContent);

        private :

            void ReadPreDefCommands_(
                s_str& sName, s_str& sParent, s_uint& uiMin, s_uint& uiMax,
                s_bool& bCopy, s_bool& bPreDefining, s_bool& bLoad, s_uint& uiRadioGroup,
                const s_bool& bMultiline
            );
        };
        friend class DefState;
        /** \endcond
        */

        void LoadDefinition_();

        void ReadTags_(s_str& sLine);
        void ReadOpeningTag_(s_str& sTagContent);
        void ReadSingleTag_(s_str& sTagContent);
        void ReadEndingTag_(s_str& sTagContent);

        s_str  sFileName_;
        s_str  sDefFileName_;
        s_str  sCurrentFileName_;
        s_uint uiCurrentLineNbr_;
        s_bool bValid_;

        XMLState     mXMLState_;
        DefState     mDefState_;
        s_ptr<State> pState_;

        // Load state
        s_ctnr<s_str> lPreProcessorCommands_;
        s_bool        bSmartComment_;
        s_str         sSmartCommentTag_;
        s_uint        uiSmartCommentCount_;
        s_bool        bMultilineComment_;
        s_uint        uiMultilineCommentCount_;
        s_bool        bPreProcessor_;
        s_uint        uiPreProcessorCount_;
        s_uint        uiSkippedPreProcessorCount_;

        Block mMainBlock_;

        s_map<s_str, Block> lPredefinedBlockList_;
    };
}
}

#endif
