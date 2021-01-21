/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            XML::Block header           */
/*                                        */
/*                                        */


#ifndef FROST_XML_BLOCK_H
#define FROST_XML_BLOCK_H

// Iterates through an XML::Block sub-blocks by name
#define foreach_named_block(block, name, parent) for ((block) = (parent)->First(name); (block); (block) = (parent)->Next())

// Iterates through an XML::Block sub-blocks
#define foreach_block(block, parent) for ((block) = (parent)->First(); (block); (block) = (parent)->Next())

#include <frost_utils_types.h>

namespace Frost
{
namespace XML
{
    class Document;
    class Block;

    enum AttrType
    {
        ATTR_TYPE_STRING,
        ATTR_TYPE_NUMBER,
        ATTR_TYPE_BOOL
    };

    /// An attribute of an XML Block
    struct Attribute
    {
        Attribute();
        Attribute(const s_str& name, const s_bool& optional = false, const s_str& def = "", const AttrType& type = ATTR_TYPE_STRING);

        s_str    sName;
        s_str    sValue;
        s_str    sDefault;
        s_bool   bOptional;
        s_bool   bFound;
        AttrType mType;

        static const s_str CLASS_NAME;
    };

    /// A wrapped pointer to a pre-defined XML Block
    struct PredefinedBlock
    {
        PredefinedBlock();
        PredefinedBlock(s_ptr<Block> block, const s_uint& min, const s_uint& max, const s_uint& radio_group = s_uint::NaN);

        s_ptr<Block> pBlock;
        s_uint       uiMin;
        s_uint       uiMax;
        s_uint       uiRadioGroup;
    };

    /// An element in an XML file
    class Block
    {
    friend class XML::Document;
    public :

        /// Default constructor.
        Block();

        /// Definition constructor.
        /** \param sName         The name of this block
        *   \param uiMinNbr      The minimum number of occurences
        *   \param uiMaxNbr      The maximum number of occurences
        *   \param sFile         The file in which this block is defined
        *   \param uiLineNbr     The line at which this block is defined
        *   \param uiRadioGroup  The radio group this Block belongs to
        */
        Block(const s_str& sName, const s_uint& uiMinNbr, const s_uint& uiMaxNbr, const s_str& sFile, const s_uint& uiLineNbr, const s_uint& uiRadioGroup = s_uint::NaN);

        /// Destructor.
        ~Block();

        /// Returns this Block's name.
        /** \return This Block's name
        */
        const s_str&  GetName() const;

        /// Returns this Block's value.
        /** \return This Block's value
        *   \note Returns an empty string if none.
        */
        const s_str&  GetValue() const;

        /// Returns this Block's parent.
        /** \return This Block's parent
        *   \note Returns NULL for the main block.
        */
        s_ptr<Block>  GetParent() const;

        /// Returns this Block's minimum number of occurences.
        /** \return This Block's minimum number of occurences
        */
        const s_uint& GetMinCount() const;

        /// Returns this Block's maximum number of occurences.
        /** \return This Block's maximum number of occurences
        */
        const s_uint& GetMaxCount() const;

        /// Checks if this Block is a radio block.
        /** \return 'true' if this Block is a radio block
        */
        s_bool        IsRadio() const;

        /// Returns the radio group this Block belongs to.
        /** \return The radio group this Block belongs to
        */
        const s_uint& GetRadioGroup() const;

        /// Checks if this Block has radio childs.
        /** \return 'true' if this Block has radio childs
        */
        const s_bool& HasRadioChilds() const;

        /// Returns the number of sub-blocks found in this Block.
        /** \return the number of sub-blocks found in this Block
        */
        s_uint        GetChildNumber() const;

        /// Returns the number of sub-blocks with a given name found in this Block.
        /** \return the number of sub-blocks with a given name found in this Block
        */
        s_uint        GetChildNumber(const s_str& sName) const;

        /// Starts iteration through this Block's sub-blocks.
        /** \param sName The name of the sub-blocks you need
        *   \return The first sub-block (NULL if none)
        *   \note If 'sName' is ommited, iteration will be
        *         done through all sub-blocks. Else, only
        *         the sub-blocks which are named 'sName'
        *         will be returned.
        */
        s_ptr<Block>  First(const s_str& sName = "");

        /// Iterates once through the sub-blocks.
        /** \return The next sub-block (NULL if none)
        *   \note See First() for more infos.
        */
        s_ptr<Block>  Next();

        /// Returns one of this Block's attribute.
        /** \param sName The name of the attribute you need
        *   \return One of this Block's attribute
        */
        s_str         GetAttribute(const s_str& sName);

        /// Checks if one of this Block's attribute has been provided.
        /** \param sName The name of the attribute to check
        *   \return 'true' if this attribute has been provided
        *   \note Calling this function is only usefull when you have
        *         optionnal attributes.
        */
        s_bool        IsProvided(const s_str& sName);

        /// Returns one of this Block's sub-block.
        /** \param sName The name of the sub-block you want
        *   \return One of this Block's sub-block
        *   \note If several sub-blocks have the same name,
        *         this function will return one of them
        *         (there is no way to predict which one).
        */
        s_ptr<Block>  GetBlock(const s_str& sName);

        /// Returns a radio block.
        /** \param uiGroup The radio block group
        *   \return A radio block
        *   \note Only works if this Block has radio childs.
        */
        s_ptr<Block>  GetRadioBlock(const s_uint& uiGroup = 0u);

        /// Returns the file into which this Block has been found.
        /** \return The file into which this Block has been found
        */
        const s_str&  GetFile() const;

        /// Returns the line at which this Block has been found.
        /** \return The line at which this Block has been found
        */
        const s_uint& GetLineNbr() const;

        /// Sets the file into which this Block has been found.
        /** \param sFile The file into which this Block has been found
        */
        void          SetFile(const s_str& sFile);

        /// Sets the line at which this Block has been found.
        /** \param uiLineNbr The line at which this Block has been found
        */
        void          SetLineNbr(const s_uint& uiLineNbr);

        static const s_str CLASS_NAME;

    protected :

        /// Copies a Block's data into another.
        /** \param pBlock The Block to copy
        */
        void          Copy(s_ptr<Block> pBlock);

        /// Adds an attribute to the list.
        /** \param mAttrib The new attribute
        *   \note Only used in the definition stage.
        */
        s_bool        Add(const Attribute& mAttrib);

        /// Removes an attribute from the available list.
        /** \param sAttributeName The name of the attribute to remove
        *   \note Only used in the definition stage.
        */
        void          RemoveAttribute(const s_str& sAttributeName);

        /// Parses attributes definitions.
        /** \param lAttribs The attribute list
        *   \note Only used in the definition stage.
        */
        void          CheckAttributesDef(const s_ctnr<s_str>& lAttribs);

        /// Retrieves attributes from a string.
        /** \param sAttributes The string containing attributes
        *   \return 'false' if an attribute is missing or the synthax
        *           is incorrect
        *   \note Only used in the loading stage.
        */
        s_bool        CheckAttributes(const s_str& sAttributes);

        /// Make sure all required blocks were found.
        /** \return 'false' if a required block wasn't found
        *           or if there was too much occurences of a
        *           particular block
        *   \note Only used in the loading stage.
        */
        s_bool        CheckBlocks();

        /// Sets this Block's name.
        /** \param sName The new name
        *   \note Only used in definition and loading stages.
        */
        void          SetName(const s_str& sName);

        /// Sets this Block's value.
        /** \param sValue The new value
        *   \note Only used in the loading stage.
        */
        void          SetValue(const s_str& sValue);

        /// Notify this Block that another one derivates from it.
        /** \param sName The name of the derivated Block
        *   \note Only works for pre-defined blocks.<br>
        *         Only used in the definition stage.
        */
        void          AddDerivated(const s_str& sName);

        /// Checks if this Block is being derivated from by another Block.
        /** \param sName The name of the Block you want to test
        *   \note Only works for pre-defined blocks.<br>
        *         Only used in the loading stage.
        */
        s_bool        HasDerivated(const s_str& sName) const;

        /// Sets this Block's parent.
        /** \param pParent The new parent
        *   \note Only used in definition and loading stages.
        */
        void          SetParent(s_ptr<Block> pParent);

        /// Sets this Block's Document.
        /** \param pDoc The Document this Block belongs to
        *   \note Only used in definition and loading stages.
        */
        void          SetDocument(s_ptr<Document> pDoc);

        /// Flags a radio group as optional.
        /** \param uiGroup The group ID
        */
        void          SetRadioGroupOptional(const s_uint& uiGroup);

        /// Returns the number of sub-blocks defined for this Block.
        /** \return the number of sub-blocks defined for this Block
        *   \note Only used in definition stage.
        */
        s_uint        GetDefChildNumber() const;

        /// Checks if this Block has another Block defined as child.
        /** \param sName The name of the Block to test
        *   \return 'true' if this Block already has the other one as child.
        *   \note Only used in definition stage.
        */
        s_bool        HasBlock(const s_str& sName);

        /// Checks if this Block can contain another Block.
        /** \param sName The name of the Block to test
        *   \return 'true' if this Block can contain the other one.
        *   \note Only used in loading stage.
        */
        s_bool        CanHaveBlock(const s_str& sName);

        /// Creates a new Block in this one.
        /** \param sName The name of the Block to create
        *   \return The new Block
        *   \note Only used in the loading stage.<br>
        *         Returns NULL if this Block is already
        *         creating another Block.<br>
        *         Doesn't check this Block can contain
        *         a new block with the provided name.
        *         See HasBlock().
        */
        s_ptr<Block>  CreateBlock(const s_str& sName);

        /// Adds the previously created Block to the list.
        /** \note Only used in the loading stage.<br>
        *         Call this function after CreateBlock(),
        *         when you're finished with it.
        */
        void          AddBlock();

        /// Creates a new Block (used for definition).
        /** \param sName    The name of the block
        *   \param uiMinNbr The minimum number of occurences
        *   \param uiMaxNbr The maximum number of occurences
        *   \return The new Block
        *   \note Only used in the definition stage.
        */
        s_ptr<Block>  CreateDefBlock(const s_str& sName, const s_uint& uiMinNbr, const s_uint& uiMaxNbr);

        /// Creates a new Block (used for definition).
        /** \param sName        The name of the block
        *   \param uiRadioGroup The radio group it belongs to
        *   \return The new Block
        *   \note Only used in the definition stage.<br>
        *         Creates a "radio" Block : it can only be present
        *         if none of the other radio blocks of the same
        *         group are present.
        */
        s_ptr<Block>  CreateRadioDefBlock(const s_str& sName, const s_uint& uiRadioGroup);

        /// Adds a pre-defined Block to the list.
        /** \param pBlock   The pre-defined Block
        *   \param uiMinNbr The minimum number of occurences
        *   \param uiMaxNbr The maximum number of occurences
        *   \return The new Block reference
        *   \note Only used in the definition stage.
        */
        s_ptr<PredefinedBlock> AddPredefinedBlock(s_ptr<Block> pBlock, const s_uint& uiMinNbr, const s_uint& uiMaxNbr);

        /// Adds a pre-defined Block to the list.
        /** \param pBlock       The pre-defined Block
        *   \param uiRadioGroup The radio group it belongs to
        *   \return The new Block reference
        *   \note Only used in the definition stage.<br>
        *         Adds a "radio" Block : it can only be present
        *         if none of the other radio blocks of the same
        *         group are present.
        */
        s_ptr<PredefinedBlock> AddPredefinedRadioBlock(s_ptr<Block> pBlock, const s_uint& uiRadioGroup);

    private :

        s_str           sName_;
        s_uint          uiMaxNumber_;
        s_uint          uiMinNumber_;
        s_uint          uiRadioGroup_;
        s_bool          bRadioChilds_;
        s_str           sValue_;
        s_ptr<Document> pDoc_;
        s_ptr<Block>    pParent_;
        s_ptr<Block>    pNewBlock_;
        s_bool          bCreating_;

        s_str  sFile_;
        s_uint uiLineNbr_;

        s_ctnr<s_multimap< s_str, s_ptr<Block> >::iterator>::iterator mCurrIter_;
        s_ctnr<s_multimap< s_str, s_ptr<Block> >::iterator>::iterator mEndIter_;

        s_ctnr<s_str> lDerivatedList_;

        s_map<s_str, Attribute>       lAttributeList_;
        s_map<s_str, Block>           lDefBlockList_;
        s_map<s_str, PredefinedBlock> lPreDefBlockList_;

        s_map< s_uint, s_ptr<Block> > lRadioBlockList_;
        s_ctnr<s_uint>                lOptionalRadioGroupList_;

        s_multimap< s_str, s_ptr<Block> >                                   lFoundBlockList_;
        s_ctnr<s_multimap< s_str, s_ptr<Block> >::iterator>                 lFoundBlockStack_;
        s_map< s_str, s_ctnr<s_multimap< s_str, s_ptr<Block> >::iterator> > lFoundBlockSortedStacks_;
    };
}
}


#endif
