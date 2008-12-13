/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_FILE_H
#define FROST_UTILS_FILE_H

#include "frost_utils.h"

namespace Frost
{
    class s_bool;
    class s_uint;
    class s_str;

    enum FileType
    {
        FILE_IO,
        FILE_I,
        FILE_O
    };

    /// A tool to input some data
    /** Built upon std::fstream, this class offers
    *   almost the exact same possibilities. The
    *   main difference is that this class can use
    *   Frost's data type.
    */
    class File
    {
    public :

        /// Call that constructor to open a file.
        /** \param sName   The file's name.
        *   \param bBinary 'false' if the file is plain text
        */
        File(const s_str& sName, const FileType& mType = FILE_IO, const s_bool& bBinary = false);

        /// Destructor.
        /** \note Automatically closes the file.
        */
        ~File();

        /// Call that function when you're done reading.
        void   Close();

        /// Checks if the file has been found and opened.
        /** \return 'true' if the file has been succesfuly opened
        */
        s_bool IsOpen();

        /// Checks if the file can still be read.
        /** \return 'true' if the file has been successfuly opened and if it
        *           has not yet reached its end.
        */
        s_bool IsValid();

        /// Returns this file's number of character.
        /** \return This file's character count
        */
        s_uint GetLength();

        /// Returns the next line.
        /** \return The next line
        */
        s_str  GetLine();

        /// Returns the reading position in the file.
        /** \return The position of the actual read character
        *   in the file
        */
        s_uint GetReadPos();

        /// Sets the reading position in the file.
        /** \param uiPos The new position
        */
        void   SetReadPos(const s_uint& uiPos);

        /// Returns the writing position in the file.
        /** \return The position of the actual written character
        *   in the file
        */
        s_uint GetWritePos();

        /// Sets the writing position in the file.
        /** \param uiPos The new position
        */
        void   SetWritePos(const s_uint& uiPos);

        /// Returns the next character.
        /** \return The next character
        */
        char   Get();

        /// Extracts a block(n-1) of unformated data until \\n is found.
        /** \param[out] sBuffer A buffer string object
        *   \param      uiSize  The number of character to extract
        */
        void   Get(s_str& sBuffer, const s_uint& uiSize);

        /// Extracts a block(n) of unformated data.
        /** \param[out] sBuffer A buffer string object
        *   \param      uiSize  The number of character to extract
        */
        void   Read(s_str& sBuffer, const s_uint& uiSize);

        /// Extracts a block(n-1) of unformated data until \\n is found.
        /** \param[out] sBuffer A buffer string object
        *   \param      uiSize  The number of character to extract
        */
        void   Get(char* sBuffer, const s_uint& uiSize);

        /// Extracts a block(n) of unformated data.
        /** \param[out] sBuffer A buffer string object
        *   \param      uiSize  The number of character to extract
        */
        void   Read(char* sBuffer, const s_uint& uiSize);

        /// Extracts a block(n-1) of unformated data until a character is found.
        /** \param[out] sBuffer A buffer string object
        *   \param      uiSize  The number of character to extract
        *   \param      cDelim  Extraction should stop when this char is encountered
        */
        void   Get(s_str& sBuffer, const s_uint& uiSize, const char& cDelim);

        /// Writes down a char array into the file.
        /** \param sBuffer The char array (can be a casted type)
        *   \param uiSize  The number of char to write to the file
        *   \note This function changes the writing position
        */
        void   Write(const char* sBuffer, const s_uint& uiSize);

        /// Writes down a single char into the file.
        /** \param cChar The char to write down
        *   \note This function changes the writing position
        */
        void   Write(const char& cChar);

        /// Forces writing everything to the file.
        void   Flush();

        /// Checks if a file exists.
        /** \param sFileName The name of the file
        *   \param bPrint    Print an error in the log file or not
        *   \return 'true' if the file exists, 'false' otherwise
        */
        static s_bool Exists(const s_str& sFileName, const s_bool& bPrint = true);

        static const s_str CLASS_NAME;

    private :

        /// The file stream object
        std::fstream mFile;
    };
}

#endif
