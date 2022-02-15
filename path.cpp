#include "path.h"
#include <algorithm>

Node::Node(Location coordinates_, std::shared_ptr<Node> parent_)
{
    parent = parent_;
    coordinates = coordinates_;
    G = H = 0;
}

AStar::AStar(uint width, uint height, Model& m_model)
    :
    m_model(m_model)
{
    grid_size = { (int)width, (int)height };
    direction = { { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 } };
}

PathOrdered& AStar::find_path(
    PathOrdered& path,
    const Location& source_,
    const Location& target_)
{
    try {
        // 检查源和目的气候类型是否一致
        uint source_terrain = get_location_terrain(source_);
        uint target_terrain = get_location_terrain(target_);
        if (source_terrain <= 1 && target_terrain > 1 ||
            target_terrain <= 1 && source_terrain > 1) {
            return path;
        }

        // 设置当前的cost_table为水生或陆生
        if (source_terrain > 1)
            cost_table = m_model.land_cost_table;
        else
            cost_table = m_model.water_cost_table;

        Node_Ptr current(nullptr);
        Node_List openSet, closedSet;
        PriorityQueue frontier;
        Node_Ptr source_node(new Node(source_));

        openSet.emplace(source_node);
        frontier.put(source_node, 0);

        while (!openSet.empty()) {
            // 获取得分最小（即cost最小）的openset中的结点作为当前所访问的结点
            // 没有用优先级队列实现
            // 这里可以用树或堆的增量排序来优化
            /*
            auto& it = openSet.begin();
            Node_Ptr current(*it);
            for (; it != openSet.end(); it++) {
                if (current->getScore() > it->get()->getScore()) {
                    current = *it;
                }
            }
            */
            current = frontier.get();   // 在获取的同时就删除了元素

            if (current->coordinates == target_) {
                // 获取路径
                while (current != nullptr) {
                    path.emplace_back(current->coordinates.x, current->coordinates.y);
                    current = current->parent;
                }
                path.pop_back();    // 最后一位是当前的位置
                /*
                for (auto& it : closedSet) {
                    path.emplace(it->coordinates);
                }
                */
                break;
            }

            closedSet.emplace(current);
            openSet.erase(current);

            for (uint i = 0; i < DIRS; ++i) {
                Location new_coordinates(current->coordinates + direction[i]);
                Node_Ptr new_node(new Node(new_coordinates));
                if (detect_border(new_coordinates) ||
                    closedSet.find(new_node) != closedSet.end()) {
                    continue;
                }

                // 计算到达当前位置tile的总花费
                // 如果花费小于0，则说明当前tile和生物类型不一致，无法行走
                int current_cost = get_cost(new_coordinates);
                if (current_cost < 0) continue;
                uint total_cost = current->G + current_cost;

                auto& it = openSet.find(new_node);
                if (it == openSet.end()) {
                    new_node->parent = current;
                    new_node->G = total_cost;
                    new_node->H = euclidean(new_node->coordinates, target_);
                    openSet.emplace(new_node);
                    frontier.put(new_node, new_node->getScore());
                }
                else if (total_cost < it->get()->G) {
                    it->get()->parent = current;
                    it->get()->G = total_cost;

                    // 将 it 指向的node加入PriQue
                    // 可能会重复，重复的情况由上面CloseSet查找cover
                    frontier.put(*it, it->get()->getScore());
                }
            }
        }
    } 
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return path;
}


bool AStar::detect_border(const Location& coordinates_) const noexcept
{
    if (coordinates_.x < 0 || coordinates_.x >= grid_size.x ||
        coordinates_.y < 0 || coordinates_.y >= grid_size.y) {
        return true;
    }
    return false;
}

int AStar::get_cost(const Location& coordinate) const
{
    uint terrain = get_location_terrain(coordinate);
    return cost_table.at(terrain);
}

uint AStar::get_location_terrain(const Location& coordinate) const
{
    tile& t = m_model.get_tile(coordinate.y * COL + coordinate.x);
    return t.get_terrain();
}

uint AStar::euclidean(const Location& source_, const Location& target_) const noexcept
{
    Location delta = { abs(source_.x - target_.x),  abs(source_.y - target_.y) };
    return sqrt(pow(delta.x, 2) + pow(delta.y, 2));
}
