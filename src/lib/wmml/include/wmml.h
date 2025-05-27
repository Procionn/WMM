#pragma once
#include "base_wmml.h"

// namespace stc {

class wmml_marker;
struct wmml_archive_struct;





class WMML_API wmml final : public base_wmml
{
public:
    wmml(const std::filesystem::path& path);
    // Opens an existing file.

    wmml(const std::filesystem::path& path, short unsigned width);
    // Creates a new file.

    ~wmml();
    // Closes the file and saves the settings.

    void             write(std::vector<variant>& writeble);
    // Constructs an object from the elements in the provided vector and appends it to the file.
    // The number of elements in the vector must match the file's width.

    bool             read(std::vector<variant>& output);
    // Reads an object from the file and stores it in the provided vector.
    // The number of elements in the vector must match the file's width.

    void             set_wmml(wmml_marker* target);
    // Writes a wmml archive to a file

    wmml_marker*     get_wmml();
    // Reads archived wmml files, returns a pointer to wmml_marker. 
    // Returns pointers starting from the last archived one. 
    // After returning the pointer to the last archived file, returns nullptr

    void             remove_object(int object_index);
    // Deletes the specified field from the object. Keeps count from zero.

    void             reset();
    // sets the object's state to its original state.

    void             flush();
    // saves cached object data to a file

    // unsigned         height(); // Inherited from wmml_base
    // Returns the number of objects in the file.

    // unsigned short   width();  // Inherited from wmml_base
    // Returns the number of fields in the object.

    template<typename T>
    void             overwriting_sector(int object_index, int sector_index, T& new_data) {
                            overwriting(    object_index,     sector_index,    new_data);}
    // A function that is strongly discouraged from use due to its instability.
    // Overwrites a selected sector of an object. If the size or type of the new data
    // does not match the existing one, an exception is thrown.
    // The user assumes all responsibility for the safe use of this method.
    // Overwrites the specified field of the selected object without changing its type and size.
    // object_index and sector_index keeps count from zero.

private:
    friend class  wmml_marker;
    friend struct wmml_archive_struct;
};   // wmml





class WMML_API wmml_marker final : public base_wmml {
    friend class wmml;
    friend class wmml_archive_struct;
    std::size_t f_mark;
    std::size_t s_mark;
    wmml* parent;
    wmml_marker(wmml* parent, unsigned long long& f_mark, unsigned long long& s_mark);
public:
    wmml_marker(const std::filesystem::path& path);
    // Opens an existing wmml file.

    // unsigned         height(); // Inherited from wmml_base
    // Returns the number of objects in the file.

    // unsigned short   width();  // Inherited from wmml_base
    // Returns the number of fields in the object.

    void unarchiving (const std::filesystem::path& unarchivedFilePath);
    // dumps the archived file assigned to an object of the wmml_marker class to disk
    // using the unarchivedFilePath.

    bool read(std::vector<variant>& output);
    // Reads data from an archived file. Completely identical to wmml::reader()
};

// }    // stc
