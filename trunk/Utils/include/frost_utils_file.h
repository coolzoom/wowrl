/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_FILE_H
#define FROST_UTILS_FILE_H

#include "frost_utils_types.h"

#include <fstream>

namespace Frost
{
    typedef char string_element;
    typedef std::string string_object;
    typedef std::stringstream string_stream;
    typedef std::fstream file_stream;

    /// A tool to input some data
    /** Built upon std::fstream, this class offers
    *   almost the exact same possibilities. The
    *   main difference is that this class can use
    *   Frost's data type.
    */
    class File
    {
    public :

        enum FileType
        {
            IO, /// Input - Output
            I,  /// Input
            O   /// Output
        };

        /// Call that constructor to open a file.
        /** \param sName   The file's name
        *   \param mType   How to open this file, see FileType
        *   \param bBinary 'false' if the file is plain text
        */
        File(const s_str& sName, const FileType& mType = IO, const s_bool& bBinary = false);

        /// Destructor.
        /** \note Automatically closes the file.
        */
        ~File();

        /// Call that function when you're done reading.
        void        Close();

        /// Checks if the file has been found and opened.
        /** \return 'true' if the file has been succesfuly opened
        */
        s_bool      IsOpen();

        /// Checks if the file can still be read.
        /** \return 'true' if the file has been successfuly opened and if it
        *           has not yet reached its end.
        */
        s_bool      IsValid();

        /// Returns this file's number of character.
        /** \return This file's character count
        */
        s_uint      GetLength();

        /// Returns this file's name.
        /** \return This file's name
        */
        const s_str GetName() const;

        /// Returns the next line.
        /** \return The next line
        */
        s_str       GetLine();

        /// Returns the reading position in the file.
        /** \return The position of the currently read character
        *   in the file
        */
        s_uint      GetReadPos();

        /// Sets the reading position in the file.
        /** \param uiPos The new position
        */
        void        SetReadPos(const s_uint& uiPos);

        /// Returns the writing position in the file.
        /** \return The position of the currently written character
        *   in the file
        */
        s_uint      GetWritePos();

        /// Sets the writing position in the file.
        /** \param uiPos The new position
        */
        void        SetWritePos(const s_uint& uiPos);

        /// Returns the next character.
        /** \return The next character
        */
        string_element Get();

        /// Extracts a block of unformated data.
        /** \param[out] sBuffer A buffer string object
        *   \param      uiSize  The number of character to extract
        */
        void        Read(s_str& sBuffer, const s_uint& uiSize);

        /// Extracts a block of unformated data.
        /** \param[out] sBuffer A buffer string object
        *   \param      uiSize  The number of character to extract
        */
        void        Read(string_element* sBuffer, const s_uint& uiSize);

        /// Extracts a block of unformated data.
        /** \param[out] pBuffer A pointer to the object/array to fill
        *   \param      uiSize  The number of character to extract
        */
        void        Read(void* pBuffer, const s_uint& uiSize);

        /// Fills an object.
        /** \param[out] mObject The object to fill
        *   \note The object provided should only contain POD types and no pointers.
        */
        template<class T>
        void        Read(T& mObject)
        {
            Read(&mObject, sizeof(T));
        }

        /// Writes down a char array into the file.
        /** \param sBuffer The char array
        *   \param uiSize  The number of char to write to the file
        *   \note This function changes the writing position
        */
        void        Write(const string_element* sBuffer, const s_uint& uiSize);

        /// Writes down some data into the file.
        /** \param pBuffer The object/array to write
        *   \param uiSize  The object's size
        *   \note This function changes the writing position
        */
        void        Write(const void* pBuffer, const s_uint& uiSize);

        /// Writes an object into the file.
        /** \param mObject The object to write
        *   \note This function changes the writing position
        */
        template<class T>
        void        Write(const T& mObject)
        {
            Write(&mObject, sizeof(T));
        }

        /// Writes down a single char into the file.
        /** \param cChar The char to write down
        *   \note This function changes the writing position
        */
        void        Write(const string_element& cChar);

        /// Writes the content of the provided string into the file.
        /** \param sLine The string to write
        */
        void        Write(const s_str& sLine);

        /// Writes the content of the provided string and go to the next line.
        /** \param sLine The string to write
        *   \note This function automatically adds the line jump.
        */
        void        WriteLine(const s_str& sLine);

        /// Forces writing everything to the file.
        void        Flush();

        /// Sets the tabulation string.
        /** \param sTabStr The tabulation string
        *   \note Default is 4 spaces : "    "
        */
        void        SetTabCharacter(const s_str& sTabStr);

        /// Returns the tabulation string.
        /** \return The tabulation string
        *   \note Default is 4 spaces : "    "
        */
        const s_str& GetTabCharacter() const;

        /// Adds a tabulation string.
        /** \note Calling this function increments the tab counter.<br>
        *         When WriteLine() is called, the tabulation string is written
        *         in front of each line, as many time as you've called this
        *         function.
        */
        void        AddTab();

        /// Removes a tabulation string.
        /** \note Calling this function decrements the tab counter.<br>
        *         See AddTab() for more infos.
        */
        void        RemoveTab();

        /// Removes all tabulation strings.
        /** \note Calling this function sets the tab counter to zero.<br>
        *         See AddTab() for more infos.
        */
        void        RemoveAllTabs();

        /// Checks if a file exists.
        /** \param sFileName The name of the file
        *   \return 'true' if the file exists, 'false' otherwise
        */
        static s_bool Exists(const s_str& sFileName);

        static const s_str CLASS_NAME;

    private :

        /// The file stream object
        file_stream mFile_;
        /// The name of this file
        s_str       sName_;
        /// The tabbing string
        s_str       sTabStr_;
        /// The total tabbing
        s_uint      uiTabCount_;
    };
}

#endif
