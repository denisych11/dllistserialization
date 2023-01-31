#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <unordered_map>
#include <vector>
#include <random>
#include <iostream>


struct ListNode {
    ListNode* prev = nullptr; // указатель на предыдущий элемент списка, либо `nullptr` в случае начала списка
    ListNode* next = nullptr;
    ListNode* rand = nullptr; // указатель на произвольный элемент  данного списка, либо `nullptr`
    std::string data; // произвольные пользовательские данные
};
class List {
public:
    void Serialize(FILE* file) // сохранение списка в файл, файл  открыт с помощью `fopen(path, "wb")`
    {
        std::unordered_map<ListNode*, int> uMap;

        auto cur = head;
        for (int i = 1; i <= count; i++)
        {
            uMap.insert(std::make_pair(cur, i));
            cur = cur->next;
        }

        std::vector <std::pair<std::string, int>> vNode;
        vNode.reserve(count);
        cur = head;
        while (cur)
        {
            int randEl{ 0 };
            if (cur->rand != nullptr)
            {
                auto search = uMap.find(cur->rand);
                randEl = search->second;
            }
            vNode.push_back(std::make_pair(cur->data, randEl));
            cur = cur->next;
        }
        
        fwrite(&count, sizeof(count), 1, file);

        for (auto& a: vNode)
        {
            fwrite(&a.second, sizeof(a.second), 1, file);
            int size = a.first.size();
            fwrite(&size, sizeof(size), 1, file);
            fwrite(a.first.c_str(), 1, size, file);
        }

    }
    void Deserialize(FILE* file) // восстановление списка из файла,   файл открыт с помощью `fopen(path, "rb")`
    {
        std::unordered_map<int, ListNode*> uMap;

        std::vector<int> vRandNode;
        int rCount{ 0 };
        fread(&rCount, sizeof(rCount), 1, file);
        vRandNode.reserve(rCount);
        rCount = 1;

        for (; rCount <= vRandNode.capacity(); rCount++)
        {
            int randNode{ 0 };
            fread(&randNode, sizeof(randNode), 1, file);

            int len{ 0 };
            fread(&len, sizeof(len), 1, file);

            std::string temp{ "" };
            while (len > 0)
            {
                char ch ;
                fread(&ch, sizeof(ch), 1, file);
                temp += ch;
                len--;
            }

            Add(temp);
            vRandNode.push_back(randNode);
            uMap.insert(std::make_pair(rCount, tail));
        }
        auto cur = head;
        for(auto a: vRandNode)
        {
            if (a != 0)
                cur->rand = uMap.find(a)->second;
            else
                cur->rand = nullptr;
            cur = cur->next;
        }
    }
    // ... ваши методы для заполнения списка
    void Add(std::string str)
    {
        ListNode* node = new ListNode;
        node->data = std::move(str);
        count++;

        if (head == nullptr)
        {
            head = node;
        }
        else
        { 
            tail->next = node;
            node->prev = tail;
        }
        tail = node;
    }
    List(){}
    List(std::string str)
    {
        Add(std::move(str));
    }
    ~List()
    {
        while (head)
        {
            auto temp = head->next;
            delete head;
            head = temp;
        }
    }
    void ShufleRandom()
    {
        std::random_device rd;   
        std::mt19937 gen(rd());

        auto cur = head;
        while (cur)
        {
            auto randNode = head;
            int randNum = gen() % (count + 1);
            for (int i = 1; i < randNum; i++)
            {
                randNode = randNode->next;
            }
            if (randNum == 0)
                cur->rand = nullptr;
            else
                cur->rand = randNode;

            cur = cur->next;
        }
    }

    void PrintListAndRand() const
    {
        auto cur = head;
        while (cur)
        {
            std::cout << "===Data->" << cur->data;
            if (cur->rand)
                std::cout << " | RandData->" << cur->rand->data << "===" << std::endl;
            else
                std::cout << " | RandData->nullptr===" << std::endl;

            cur = cur->next;
        }
        std::cout << "_______________________________________________________________" << std::endl;
    }
private:
    ListNode* head = nullptr;
    ListNode* tail = nullptr;
    int count = 0;
};


int main()
{
    List list1("abra");
    list1.Add("kadabra");
    list1.Add("sim");
    list1.Add("salabim");
    list1.Add("a  tut     ");
    list1.Add("bibala");
    list1.Add("        pobvoh");
    list1.Add("pup");
    list1.ShufleRandom();
    std::cout << "list1" << std::endl;
    list1.PrintListAndRand();

    FILE* file;
    if ((file = fopen("file.bin", "wb")) != nullptr)
    {
        list1.Serialize(file);
        fclose(file);
        List list2;
        if ((file = fopen("file.bin", "rb")) != nullptr)
        {
            list2.Deserialize(file);
            fclose(file);
            std::cout << "list2" << std::endl;
            list2.PrintListAndRand();
        }
    }    
}