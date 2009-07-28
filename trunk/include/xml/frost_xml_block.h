/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               XXX header               */
/*                                        */
/*                                        */


#ifndef FROST_XML_BLOCK_H
#define FROST_XML_BLOCK_H

// Iterates through an XML::Block sub-blocks by name
#define foreach_named_block(block, name, parent) for ((block) = (parent)->First(name); (block); (block) = (parent)->Next())

// Iterates through an XML::Block sub-blocks
#define foreach_block(block, parent) for ((block) = (parent)->First(); (block); (block) = (parent)->Next())

#include "frost.h"

namespace Frost
{
namespace XML
{
    class Document;

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
        PredefinedBlock(s_ptr<Block> block, const s_uint& min, const s_uint& max, const s_bool& radio = false);

        s_ptr<Block> pBlock;
        s_uint       uiMin;
        s_uint       uiMax;
        s_bool       bRadio;
    };

    /// An element in an XML file
    class Block
    {
    public :

        /// Default constructor.
        Block();

        /// Definition constructor.
        /** \param sName     The name of this block
        *   \param uiMinNbr  The minimum number of occurences
        *   \param uiMaxNbr  The maximum number of occurences
        *   \param sFile     The file in which this block is defined
        *   \param uiLineNbr The line at which this block is defined
        *   \param bRadio    'true' if only one Block can be present on this level
        */
        Block(const s_str& sName, const s_uint& uiMinNbr, const s_uint& uiMaxNbr, const s_str& sFile, const s_uint& uiLineNbr, const s_bool& bRadio = false);

        /// Copy constructor.
        /** \param mValue The Block to copy
        */
        Block(const Block& mValue);

        /// Destructor.
        ~Block();

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

        /// Returns this Block's name.
        /** \return This Block's name
        */
        const s_str&  GetName() const;

        /// Sets this Block's name.
        /** \param sName The new name
        *   \note Only used in definition and loading stages.
        */
        void          SetName(const s_str& sName);

        /// Returns this Block's value.
        /** \return This Block's value
        *   \note Returns an empty string if none.
        */
        const s_str&  GetValue() const;

        /// Sets this Block's value.
        /** \param sValue The new value
        *   \note Only used in the loading stage.
        */
        void          SetValue(const s_str& sValue);

        /// Returns this Block's parent.
        /** \return This Block's parent
        *   \note Returns NULL for the main block.
        */
        s_ptr<Block>  GetParent() const;

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
        s_bool        HasDerivated(const s_str& sName);

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

        /// Returns this Block's minimum number of occurences.
        /** \return This Block's minimum number of occurences
        */
        const s_uint& GetMinCount() const;

        /// Returns this Block's maximum number of occurences.
        /** \return This Block's maximum number of occurences
        */
        const s_uint& GetMaxCount() const;

        /// Makes this Block a "radio" Block.
        /** \note Only used in the definition stage.
        */
        void          SetRadio();

        /// Checks if this Block can only be present if it's alone on its level.
        /** \return 'true' if this Block can only be present if it's alone on its level
        */
        const s_bool& IsRadio() const;

        /// Checks if this Block can only contain one child.
        /** \return 'true' if this Block can only contain one child
        */
        const s_bool& HasRadioChilds() const;

        /// Returns the number of sub-blocks defined for this Block.
        /** \return the number of sub-blocks defined for this Block
        *   \note Only used in definition stage.
        */
        s_uint        GetDefChildNumber() const;

        /// Returns the number of sub-blocks found in this Block.
        /** \return the number of sub-blocks found in this Block
        *   \note Only used in loading stage.
        */
        s_uint        GetChildNumber() const;

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

        /// Returns the only sub-block available.
        /** \return The only sub-block available
        *   \note Only works if this Block has radio childs.
        */
        s_ptr<Block>  GetRadioBlock();

        /// Checks if this Block can contain another Block.
        /** \param sName The name of the Block to test
        *   \return 'true' if this Block can contain the other one.
        *   \note Only used in definition and loading stages.
        */
        s_bool        HasBlock(const s_str& sName);

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
        /** \param sName The name of the block
        *   \return The new Block
        *   \note Only used in the definition stage.<br>
        *         Creates a "radio" Block : it can only be present
        *         if none of its defined neighbours are.
        */
        s_ptr<Block>  CreateRadioDefBlock(const s_str& sName);

        /// Adds a pre-defined Block to the list.
        /** \param pBlock   The pre-defined Block
        *   \param uiMinNbr The minimum number of occurences
        *   \param uiMaxNbr The maximum number of occurences
        *   \return The new Block reference
        *   \note Only used in the definition stage.
        */
        s_ptr<PredefinedBlock> AddPredefinedBlock(s_ptr<Block> pBlock, const s_uint& uiMinNbr, const s_uint& uiMaxNbr);

        /// Adds a pre-defined Block to the list.
        /** \param pBlock   The pre-defined Block
        *   \return The new Block reference
        *   \note Only used in the definition stage.<br>
        *         Adds a "radio" Block : it can only be present
        *         if none of its defined neighbours are.
        */
        s_ptr<PredefinedBlock> AddPredefinedRadioBlock(s_ptr<Block> pBlock);

        /// Returns the file into which this Block has been found.
        /** \return The file into which this Block has been found
        */
        const s_str& GetFile() const;

        /// Returns the line at which this Block has been found.
        /** \return The line at which this Block has been found
        */
        const s_uint& GetLineNbr() const;

        static const s_str CLASS_NAME;

    private :

        s_str           sName_;
        s_uint          uiMaxNumber_;
        s_uint          uiMinNumber_;
        s_bool          bRadio_;
        s_bool          bRadioChilds_;
        s_str           sValue_;
        s_ptr<Document> pDoc_;
        s_ptr<Block>    pParent_;
        s_refptr<Block> pNewBlock_;
        s_bool          bCreating_;

        s_str  sFile_;
        s_uint uiLineNbr_;

        s_ctnr<s_multimap< s_str, s_ptr<Block> >::iterator>::iterator mCurrIter_;
        s_ctnr<s_multimap< s_str, s_ptr<Block> >::iterator>::iterator mEndIter_;

        s_ctnr<s_str> lDerivatedList_;

        s_map<s_str, Attribute>       lAttributeList_;
        s_map<s_str, Block>           lDefBlockList_;
        s_map<s_str, PredefinedBlock> lPreDefBlockList_;

        s_multimap< s_str, s_ptr<Block> >                                   lFoundBlockList_;
        s_ctnr<s_multimap< s_str, s_ptr<Block> >::iterator>                 lFoundBlockStack_;
        s_map< s_str, s_ctnr<s_multimap< s_str, s_ptr<Block> >::iterator> > lFoundBlockSortedStacks_;
    };
}
}


#endif
