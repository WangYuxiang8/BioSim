#pragma once
#include <vector>
#include <unordered_set>
#include <memory>
#include <queue>
#include "model.h"
#include "view.h"
#include "config.h"
/*
����λ�ýṹ�壬������ʾtile������
����==�����������Ϊ������������
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

/* Location �ṹ����б����ͣ��洢����·�� */
typedef std::unordered_set<Location, Location_Hash> Coordinate_List;

/*
������ṹ�壬�����ʾΪһ������ͼ�Ľ��
��parent�洢��һ�ξ����Ľ�㣬��������·��
*/
struct Node
{
    uint G, H;                      // G��ʾ�ӳ����㿪ʼ��cost���ѣ�H��ʾ����Ŀ���������٣�ŷʽ������
    Location coordinates;           // ��ǰ�������
    std::shared_ptr<Node> parent;   // ��¼�������������һ�����

    Node(Location coord_, std::shared_ptr<Node> parent_ = nullptr);
    inline uint getScore()
        // ���cost��ԽС�������ȼ�Խ�ߣ�Ӧ�����ȷ���
    {
        uint w = (H / W_RATIO) + 1;
        return G + w * H;
    }
};

/* Node �ṹ�������ָ������ */
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

/* �洢Node_Ptrָ��Ĺ�ϣ�� */
typedef std::unordered_set<Node_Ptr, Node_Point_Hash, Node_Point_Eq> Node_List;

/*
���ȶ��У�������С��
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
    
    /* A star �����㷨 */
    PathOrdered& find_path(
        PathOrdered& path,
        const Location& source_,
        const Location& target_);

private:
    Model& m_model;

    std::vector<Location> direction;
    std::vector<int> cost_table;
    Location grid_size;
    const uint DIRS = 4;    // �ĸ�����

    /* �������Ŀ����ŷ����þ��� */
    uint euclidean(const Location& source_, const Location& target_) const noexcept;

    /* ��⵱ǰ�����Ƿ񳬹��߽� */
    bool detect_border(const Location& coordinates_) const noexcept;

    /* ��ȡ��ǰ����Ļ���cost */
    int get_cost(const Location& coordinate) const;

    /* ��ȡ���������Ӧ��tile���������� */
    uint get_location_terrain(const Location& coordinate) const;
};