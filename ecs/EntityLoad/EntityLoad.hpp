#pragma once

#include <map>
#include <memory>
#include <dlfcn.h>
#include <boost/filesystem.hpp>
#include <ctime>

#ifdef CLIENT
    #include "Graphics.hpp"
#endif

#include "AComponents.hpp"
#include "Error.hpp"

class EntityLoad
{
    public:
        /**
         * @brief Construct a new Entity Load object from shared librairies
         *
         */
        EntityLoad();
        /**
         * @brief Destroy the Entity Load object
         *
         */
        ~EntityLoad();
        /**
         * @brief load an entitiys from shared libraries
         *
         * @param game entity manager
         */
        void loadEntity(Registry &game);
        /**
         * @brief reaload all entity from shared object
         *
         * @param game entity manager
         */
        void reloadAllEntitys(Registry &game);
        /**
         * @brief reload one entity from the shared object
         *
         * @param entityName name of the entity you want to reload
         * @param game entity manager
         */
        void reloadEntity(std::string const &entityName, Registry &game);
#ifdef CLIENT
        /**
         * @brief spwan loaded entity
         *
         * @param entityName filename of the entity we must spawn
         * @param game entity manager
         * @param resources graphical component
         * @return std::size_t
         */
        std::size_t spawnEntity(std::string const &entityName, Registry &game, Resource::Graphics &resources);
#else   /**
        * @brief select a random entity already load and spawn it
        *
        * @param game entity manager
        * @return std::size_t
        */
        std::size_t spawnRandomEntity(Registry &game);
        /**
         * @brief spawn the selected entity
         *
         * @param entityName name of the entity we must spawn
         * @param game entity manager
         * @return std::size_t
         */
        std::size_t spawnEntity(std::string const &entityName, Registry &game);
#endif
        /**
         * @brief Get the Entity object
         *
         * @return std::shared_ptr<Shared::AComponents>
         */
        std::shared_ptr<Shared::AComponents> getEntity(const std::string &);
        /**
         * @brief Get the Names object
         *
         * @return std::vector<std::string>
         */
        std::vector<std::string> getNames();
        /**
         * @brief Get the Id object
         *
         * @param entityName name of the entity we must spawn
         * @return std::vector<std::size_t>
         */
        std::vector<std::size_t> getId(std::string const &entityName);
        /**
         * @brief Get the Last Spawn object
         *
         * @return const std::pair<std::string, std::size_t>&
         */
        const std::pair<std::string, std::size_t> &getLastSpawn() const {
            return _lastSpawn;
        }

    private:
        std::map<std::string, std::shared_ptr<Shared::AComponents>> _entityLoad;
        std::map<std::string, std::vector<std::size_t>> _entityId;
        std::vector<boost::filesystem::path> _paths;
        std::pair<std::string, std::size_t> _lastSpawn;
};