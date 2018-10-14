/*!
 * \author ddubois
 * \date 14-Aug-18.
 */

#ifndef NOVA_RENDERER_RESOURCEPACK_H
#define NOVA_RENDERER_RESOURCEPACK_H

#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <optional>
#include "../util/utils.hpp"

#if _MSC_VER <= 1915
namespace fs = std::experimental::filesystem;
#else
namespace fs = std::filesystem;
#endif

namespace nova {
    NOVA_EXCEPTION(resource_not_found_exception);

    class filesystem_exception : public std::exception {  // Convert fs::filesystem_error into a nova class
    private:
        const std::string message;
        const std::error_code error_code;

    public:
        explicit filesystem_exception(const fs::filesystem_error &error) : message(error.what()), error_code(error.code()) {}

        const char *what() const noexcept override {
            return message.c_str();
        }

        const std::error_code code() const noexcept {
            return error_code;
        }
    };

    /*!
     * \brief A collection of resources on the filsysstem
     *
     * "resourcepack" isn't the exact right name here. This isn't strictly a resourcepack in the Minecraft sense - it
     * can be, sure, but it can also be a pure shaderpack. Ths main point is to abstract away loading resources from a
     * folder or a zip file - the calling code shouldn't care how the data is stored on the filesystem
     */
    class folder_accessor_base {
    public:
        /*!
         * \brief Initializes this resourcepack to load resources from the folder/zip file with the provided name
         * \param folder The name of the folder or zip file to load resources from, relative to Nova's working directory
         */
        explicit folder_accessor_base(const fs::path &folder);

        // Else freeing could cause errors
        virtual ~folder_accessor_base() {};

        /*!
         * \brief Checks if the given resource exists
         * \param resource_path The path to the resource you want to know the existence of, relative to this
         * resourcepack's root
         * \return True if the resource exists, false if it does not
         */
        bool does_resource_exist(const fs::path &resource_path);

        /*!
         * \brief Loads the resource with the given path
         * \param resource_path The path to the resource to load, relative to this resourcepack's root
         * \return All the bytes in the loaded resource
         */
        virtual std::string read_text_file(const fs::path &resource_path) = 0;

        /*!
         * \brief Loads the file at the provided path as a series of 32-bit numbers
         * 
         * \param resource_path The path to the SPIR-V file to load, relative to this resourcepack's root
         * \return All the 32-bit numbers in the SPIR-V file
         */
        std::vector<uint32_t> read_spirv_file(fs::path& resource_path);

        /*!
         * \brief Retrieves the paths of all the items in the specified folder
         * \param folder The folder to get all items from
         * \return A list of all the paths in the provided folder
         */
        virtual std::vector<fs::path> get_all_items_in_folder(const fs::path &folder) = 0;

        const fs::path &get_root() const;

    protected:
        fs::path root_folder;

        /*!
         * \brief I expect certain resources, like textures, to be requested a lot as Nova streams them in and out of
         * VRAM. This map caches if a resource exists or not - if a path is absent from the map, it's never been
         * requested and we don't know if it exists. However, if a path has been checked before, we can now save an IO
         * call!
         */
        std::unordered_map<std::string, bool> resource_existence;

        std::optional<bool> does_resource_exist_in_map(const std::string &resource_string) const;

        /*!
         * \brief Like the non-internal one, but does not add the folder's root to resource_path
         *
         * \param resource_path The path to the resource, with `our_root` already appended
         */
        virtual bool does_resource_exist_internal(const fs::path &resource_path) = 0;
    };

    /*!
     * \brief Checks if the given path has the other path as its root
     * \param path The path to check if it has the root
     * \param root The potential root path of the file
     * \return True if `path` has `root` as its root, false otherwise
     */
    bool has_root(const fs::path &path, const fs::path &root);
}  // namespace nova

#endif  // NOVA_RENDERER_RESOURCEPACK_H
