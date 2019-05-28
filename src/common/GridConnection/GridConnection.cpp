#include "GridConnection.hpp"

Grid::~Grid()
{
}

void GridConnectionManager::registerGrid(Grid* grid)
{
    grids.insert(grid);

    // Check if there's a consumer with a saved connection looking for this grid
    for (auto consumer : consumers)
    {
        if (!isConnected(consumer) && grid->getDevice().id == consumer->gridGetLastDeviceId())
        {
            connect(grid, consumer);
            return;
        }
    }
}

void GridConnectionManager::registerGridConsumer(GridConsumer* consumer)
{
    consumers.insert(consumer);

    // Check if this consumer had a saved connection to an already-registered grid
    for (auto grid : grids)
    {
        if (consumer->gridGetLastDeviceId() == grid->getDevice().id)
        {
            connect(grid, consumer);
            return;
        }
    }
}

void GridConnectionManager::deregisterGrid(std::string id, bool deleteGrid)
{
    for (Grid* grid : grids)
    {
        if (grid->getDevice().id == id)
        {
            disconnect(grid);
            grids.erase(grid);
            if (deleteGrid)
            {
                delete grid;
            }
            return;
        }
    }
}

void GridConnectionManager::deregisterGridConsumer(GridConsumer* consumer)
{
    disconnect(consumer);
    consumers.erase(consumer);
}

void GridConnectionManager::connect(Grid* grid, GridConsumer* consumer)
{
    auto iter = consumerToGridMap.find(consumer);
    if (iter != consumerToGridMap.end() && iter->second == grid)
    {
        // This connection is already established, ignore.
        return;
    }
    disconnect(consumer);
    disconnect(grid);
    consumerToGridMap[consumer] = grid;
    idToConsumerMap[grid->getDevice().id] = consumer;
    consumer->gridConnected(grid);
}

bool GridConnectionManager::isConnected(GridConsumer* consumer)
{
    return consumerToGridMap.find(consumer) != consumerToGridMap.end();
}

void GridConnectionManager::disconnect(Grid* grid)
{
    auto iter = idToConsumerMap.find(grid->getDevice().id);
    if (iter != idToConsumerMap.end())
    {
        GridConsumer* consumer = iter->second;
        consumer->gridDisconnected();
        idToConsumerMap.erase(grid->getDevice().id);
        consumerToGridMap.erase(consumer);
    }
}

void GridConnectionManager::disconnect(GridConsumer* consumer)
{
    auto iter = consumerToGridMap.find(consumer);
    if (iter != consumerToGridMap.end())
    {
        Grid* grid = iter->second;
        grid->clearAll();
        consumer->gridDisconnected();
        consumerToGridMap.erase(consumer);
        idToConsumerMap.erase(grid->getDevice().id);
    }
}

void GridConnectionManager::dispatchButtonMessage(MonomeDevice* device, int x, int y, bool state)
{
    auto iter = idToConsumerMap.find(device->id);
    if (iter != idToConsumerMap.end())
    {
        iter->second->gridButtonEvent(x, y, state);
    }
}

const std::set<Grid*>& GridConnectionManager::getGrids()
{
    return grids;
}

GridConnectionManager::GridConnectionManager()
{
}

GridConnectionManager* GridConnectionManager::instance = nullptr;
GridConnectionManager* GridConnectionManager::get()
{
    if (!instance)
    {
        instance = new GridConnectionManager();
    }
    return instance;
}
