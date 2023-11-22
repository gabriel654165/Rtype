#include "EntityLoad.hpp"

EntityLoad::EntityLoad()
{
}

EntityLoad::~EntityLoad()
{
}

std::shared_ptr<Shared::AComponents> EntityLoad::getEntity(const std::string &entityName)
{
    auto entity = _entityLoad.find(entityName);
    if (entity != _entityLoad.end())
        return entity->second;
    std::cout << "try to get not loaded entity\n";
    return nullptr;
}

std::vector<std::size_t> EntityLoad::getId(const std::string &entityName)
{
    auto entity = _entityId.find(entityName);
    if (entity != _entityId.end())
        return entity->second;
    std::cout << "try to get not loaded entity\n";
    return {};
}

void extractFileName(std::string &mainStr, const std::string &toErase)
{
    std::size_t pos = mainStr.find(toErase);
    std::size_t found = mainStr.find_last_of("/");


    if (pos != std::string::npos)
        mainStr.erase(pos, toErase.length());
    if (found != std::string::npos)
        mainStr.erase(0, found + 1);
}

void EntityLoad::loadEntity(Registry &game)
{
    boost::filesystem::path path("build/lib");
    std::string fileName;
    std::vector<std::size_t> entityIds;

    try
    {
        if (boost::filesystem::exists(path) && boost::filesystem::is_directory(path)) {
            copy(boost::filesystem::directory_iterator(path), boost::filesystem::directory_iterator(), back_inserter(_paths));
            for (const auto &it : _paths) {
                fileName = it.c_str();
                extractFileName(fileName, ".so");
                std::cout << "Entry = " << fileName << '\n';
                if (_entityLoad.find(fileName) != _entityLoad.end())
                    std::cout << "Key found\n";
                else {
                    void *handle = dlopen(it.c_str(), RTLD_NOW | RTLD_GLOBAL);
                    if (!handle)
                        throw (err::LoaderException(dlerror()));
                    std::shared_ptr<Shared::AComponents>(*entrypoint)(Registry &) = (decltype(entrypoint))dlsym(handle, "getComponent");
                    if (!entrypoint)
                        throw (err::LoaderException(dlerror()));
                    std::shared_ptr<Shared::AComponents> entity = entrypoint(game);
                    _entityLoad.insert(std::make_pair(fileName, entity));
                    _entityId.insert(std::make_pair(fileName, entityIds));
                }
            }
        } else {
            std::cout << "ERROR: please verify pass from shared entity lib\n";
        }
    }
    catch (const boost::filesystem::filesystem_error &er)
    {
        std::cerr << er.what() << '\n';
    }
}


std::vector<std::string> EntityLoad::getNames()
{
    std::vector<std::string> names;

    for (const auto &entity : _entityLoad)
        names.push_back(entity.first);
    return names;
}


void EntityLoad::reloadAllEntitys(Registry &game)
{
    _entityLoad.clear();
    loadEntity(game);
}

void EntityLoad::reloadEntity(std::string const &entityName, Registry &game)
{
    _entityLoad.erase(entityName);
    loadEntity(game);
}

#ifdef CLIENT
std::size_t EntityLoad::spawnEntity(std::string const &entityName, Registry &game, Resource::Graphics &resources) {
    auto entity = getEntity(entityName);
    if (entity == nullptr) {
        std::cout << "try to spawn not loaded entity\n";
        return 0;
    }
    std::cout << "je print le meilleur compoenent\n";
    std::size_t entityId = entity->spawnEntity();
    auto animProp = entity->getAnimationProper();
    auto rectProp = entity->getRectProper();
    auto &spriteTmp = game.emplace_component<Sprite>(entityId);
    auto &enemiesAnimation = game.emplace_component<Animations>(entityId);
    auto &playerTexture = resources.getTexture(Resource::Graphics::Textures(entity->getText()));
    spriteTmp->sprite.setTexture(playerTexture);
    spriteTmp->sprite.setTextureRect(sf::IntRect(rectProp.rectLeft, rectProp.rectTop, rectProp.rectWidth, rectProp.rectHeight));
    enemiesAnimation->animations.emplace("BasicAnim", AnimationData{
        .texture = playerTexture,
        .nextFrameOffset = animProp.nextFrameOffset,
        .firstFramePosition = animProp.firstFramePosition,
        .frames = animProp.frames,
        .currentFrame = 0,
        .timeStep = animProp.timeStep,
    });
    enemiesAnimation->currentAnimation = enemiesAnimation->animations.begin();
    enemiesAnimation->active = true;
    enemiesAnimation->loop = true;
    _entityId[entityName].emplace_back(entityId);
    return entityId;
}
#else

std::size_t EntityLoad::spawnRandomEntity(Registry &game) {
    std::size_t entityid = 0;
    srand((unsigned) time(0));
    //std::cout << "size of _path = " << _paths.size() << "\n";
    auto number = rand()%((_paths.size() - 1)- 0 + 1) + 0;
    //std::cout << "number = " << number << "\n";
    std::string file = _paths.at(number).c_str();
    extractFileName(file, ".so");
    //std::cout << "file is = " << file << "\n";
    entityid = spawnEntity(file, game);
    _lastSpawn.first = file;
    _lastSpawn.second = entityid;
    return entityid;
}

std::size_t EntityLoad::spawnEntity(std::string const &entityName, Registry &game) {
    auto entity = getEntity(entityName);
    if (entity == nullptr) {
        std::cout << "try to spawn not loaded entity\n";
        return 0;
    }
    std::size_t entityId = entity->spawnEntity();
    _entityId[entityName].emplace_back(entityId);
    return entityId;
}
#endif