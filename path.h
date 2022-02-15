#pragma once
#include <vector>
#include <unordered_set>
#include <memory>
#include <queue>
#include "model.h"
#include "view.h"
#include "config.h"
/*
定义位置结构体，用来表示tile的坐标
重载==运算符，定义为横纵坐标均相等
*/
struct Location
{
    int x, y;

    inline bool operator == (const Location& coordinates_) const
    {
        return (x == coordinates_.x && y == coordinates_.y);
    }
    inline Location operator + (const Location& other)
    {
        return{ x + other.x, y + other.y };
    }
};

struct Location_Hash {
    std::size_t operator () (Location const& loc) const noexcept {
        return std::hash<int>()(loc.x ^ (loc.y << 4));
    }
};

/* Location 结构体的列表类型，存储返回路径 */
typedef std::unordered_set<Location, Location_Hash> Coordinate_List;

/*
定义结点结构体，抽象表示为一个网格图的结点
用parent存储上一次经过的结点，用来连接路径
*/
struct Node
{
    uint G, H;                      // G表示从出发点开始的cost花费，H表示距离目标点的曼哈顿（欧式）距离
    Location coordinates;           // 当前点的坐标
    std::shared_ptr<Node> parent;   // 记录本结点来自于上一个结点

    Node(Location coord_, std::shared_ptr<Node> parent_ = nullptr);
    inline uint getScore()
        // 混合cost，越小表明优先级越高，应当首先访问
    {
        uint w = (H / W_RATIO) + 1;
        return G + w * H;
    }
};

/* Node 结构体的智能指针类型 */
typedef std::shared_ptr<Node> Node_Ptr;

struct Node_Point_Hash {
    std::size_t operator() (Node_Ptr const& n) const noexcept {
        return std::hash<int>()(n->coordinates.x ^ (n->coordinates.y << 4));
    }
};

struct Node_Point_Eq {
    bool operator () (Node_Ptr const& lhs, Node_Ptr const& rhs) const noexcept {
        return lhs->coordinates == rhs->coordinates;
    }
};

/* 存储Node_Ptr指针的哈希表 */
typedef std::unordered_set<Node_Ptr, Node_Point_Hash, Node_Point_Eq> Node_List;

/*
优先队列，采用最小堆
*/
struct PriorityQueue {
    typedef std::pair<uint, Node_Ptr> PQElement;
    std::priority_queue<PQElement, std::vector<PQElement>,
        std::greater<PQElement>> elements;

    inline bool empty() const {
        return elements.empty();
    }

    inline void put(Node_Ptr item, uint priority) {
        elements.emplace(priority, item);
    }

    Node_Ptr get() {
        Node_Ptr best_item = elements.top().second;
        elements.pop();
        return best_item;
    }
};

class AStar
{
public:
    AStar(uint width, uint height, Model& m_model);
    
    /* A star 核心算法 */
    PathOrdered& find_path(
        PathOrdered& path,
        const Location& source_,
        const Location& target_);

private:
    Model& m_model;

    std::vector<Location> direction;
    std::vector<int> cost_table;
    Location grid_size;
    const uint DIRS = 4;    // 四个方向

    /* 计算距离目标点的欧几里得距离 */
    uint euclidean(const Location& source_, const Location& target_) const noexcept;

    /* 检测当前坐标是否超过边界 */
    bool detect_border(const Location& coordinates_) const noexcept;

    /* 获取当前坐标的花费cost */
    int get_cost(const Location& coordinate) const;

    /* 获取输入坐标对应的tile的气候类型 */
    uint get_location_terrain(const Location& coordinate) const;
};