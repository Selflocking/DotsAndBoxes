#include "UCT.h"
#include "Node.h"
#include "board.h"
#include "define.h"
#include "fun.h"
#include <random>
#include <thread>

Node *root;
void Expand(Node *node)
{
    node->board.traverseEdge([&node](LOC l) {
        if (node->board.earnBox(l))
        {
            Node * t = new Node(node->board, -node->owner, node, l);
            ExpandEx(t);
            delete t;
        }
        else
        {
            node->children.emplace_back(Node{node->board, -node->owner, node, l});
        }
    });
}

void ExpandEx(Node *node)
{
    if(node->board.winner()!=EMPTY) return ;
    
    node->board.traverseEdge([&node](LOC l) {
        if (node->board.earnBox(l))
        {
            Node * t = new Node(*node, l);
            ExpandEx(t);
            delete t;
        }
        else
        {
            node->parent->children.emplace_back(Node{*node, l});
            if(node->parent->children[node->parent->children.size()-1].owner!=-node->parent->owner)
            {
                cout<<"error ExpandEx owner worng"<<endl;
            }
        }
    });
}

bool TreePolicy(Node *node)
{
    while (node->board.winner() == EMPTY)
    {
        if (node->children.empty())
        {
            Expand(node);
            int a = node->children.size()/THREAD;
            int b = node->children.size()%THREAD;
            for(int i = 0;i<a;i++)
            {
                std::thread t[THREAD];
                for(int j = 0;j<THREAD;j++)
                {
                    t[j] = std::thread(play, &node->children[i*THREAD+j]);
                }
                for(int j = 0;j<THREAD;j++)
                {
                    t[j].join();
                }
            }
            if(b!=0){
                std::thread t[b];
                for(int i = 0;i<b;i++)
                {
                    t[i] = std::thread(play, &node->children[a*THREAD+i]);
                }
                for(int i = 0;i<b;i++)
                {
                    t[i].join();
                }
            }
            // for (auto &it : node->children)
            // {
            //     play(&it);
            // }
            return false;
        }
        else
        {
            double max = -1e5;
            Node *next = &node->children[0];
            for (auto &i : node->children)
            {
                i.UCB(root->total);
                if (i.total == 0)
                    cout << "thread dont run" << endl;
                if (i.value > max)
                {
                    max = i.value;
                    next = &i;
                }
            }
            node = next;
        }
    }
    return true;
}

vector<LOC> *UCTSearch(Board *board, int owner)
{
    root = new Node(*board, owner);

    time_t begin = time(nullptr);
    while (time(nullptr) - begin < TIME)
    {
        //        for (int i = 0; i < EXPAND; ++i)

        if (TreePolicy(root))
            break;
        //                goto Result;
        //        }
    }
    // Result:
    double max = -1e5;
    double res_win = -1e5;

    double max_win = -1e5;
    double max_win_val = -1e5;
    Node *res;
    cout << "children: " << root->children.size() << endl;
    cout << "total: " << root->total << endl;
    for (auto &it : root->children)
    {
        it.UCB(root->total);
        double wi = (double)it.win / it.total;;
        if(it.action.size()>1){
            cout<<"more action: "<<it.action.size()<<" "<<it.value<<" "<<wi<<endl;
        }
        if (it.value > max)
        {
            res = &it;
            max = it.value;
            res_win = wi;
        }
        if (wi > max_win)
        {
            max_win = wi;
            max_win_val = it.value;
        }
    }
    cout << "Max UCB value: " << max << " " << res_win * 100 << "%" << endl;
    cout << "Max win: " << max_win_val << " " << max_win * 100 << "%" << endl;
    auto bak = new vector<LOC>(res->action);
    delete root;
    return bak;
}

double UCB(int win, int total, int n)
{
    return (double)win / total + C * sqrt(2 * log(n) / total);
}

void play(Node *node)
{
    int win = 0;
    int cnt = -1;
    vector<LOC> lines;
    node->board.traverseEdge([&lines](LOC l) { lines.emplace_back(l); });
    Node t;
    vector<LOC> lt;
    int BB = node->board.blackBox;
    int WB = node->board.whiteBox;
    while ((++cnt) < PLAY)
    {
        t = *node;
        lt = lines;
        // 置乱边集
        std::shuffle(lt.begin(), lt.end(), std::mt19937(std::random_device()()));

        // 当没有下完时，轮流着棋
        while (!lt.empty())
        {
            // 当还可以着边，并且所着边占据了格子时，连续着边
            while (!lt.empty() && t.board.occLine(t.owner, lt.back()))
            {
                lt.pop_back();
            }
            // 弹出已经被下的边
            if (!lt.empty())
            {
                lt.pop_back();
            }
            // 交换下棋方
            t.owner = -t.owner;
        }
        if(t.board.blackBox-BB>t.board.whiteBox-WB){
            if(node->owner==BLACK){
                ++win;
            }else{
                --win;
            }
        }else if(t.board.blackBox-BB<t.board.whiteBox-WB){
            if(node->owner==WHITE){
                ++win;
            }else{
                --win;
            }
        }
        // 计算胜利与否
        // if (t.board.winner() == node->owner)
        // {
        //     ++win;
        // }
    }
    node->win =  - win;
    node->total = PLAY;
    BackUp(node, node->win, node->total);
}

void BackUp(Node *node, int win, int total)
{
    Node *t = node;
    while (node->parent != nullptr)
    {
        node = node->parent;
        node->total += total;
        if (node->owner == t->owner)
            node->win += win;
        else
            node->win += - win;
    }
}
void UCT(Board *board, int owner)
{
    time_t begin = time(nullptr); // 记录开始时间

    string res; // 引擎需要
    auto l = UCTSearch(board, owner);

    if (l->empty()){
        cout<<"no move"<<endl;
        return;
    }

    for (auto &it : *l)
    {
        board->occLine(owner, it);
        res += change(it);
    }
    delete l;
    time_t cost = time(nullptr) - begin; // 计算计算时间
    if (cost > 60)
    {
        printf("cost: %2d min %2d sec\n", cost / 60, cost % 60);
    }
    else
    {
        printf("cost: %2d sec\n", cost);
    }

    cout << "move " << res.size() / 3 << " " << res << endl;
}