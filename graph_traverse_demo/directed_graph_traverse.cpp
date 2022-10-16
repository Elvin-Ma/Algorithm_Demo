#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <stack>

class Node {
public:
  Node() = default;
  Node(std::string name, int32_t value): value_(value), name_(name) {}
  Node(Node* left, Node* right): left_(left), right_(right) {}
  Node(int32_t value, std::string name, Node* left, Node* right):
    value_(value), name_(name), left_(left), right_(right) {}

  Node(const Node& node) = delete;
  Node& operator=(const Node&) = delete;

  Node(Node&& node) {
    this->value_ = node.value_;
    this->name_ = node.name_;
    this->left_ = node.left_;
    this->right_ = node.right_;

    node.value_ = 0;
    node.name_ = "";
    node.left_ = nullptr;
    node.right_ = nullptr;
  }
  Node& operator=(Node&& node) {
    this->value_ = node.value_;
    this->name_ = node.name_;
    this->left_ = node.left_;
    this->right_ = node.right_;

    node.value_ = 0;
    node.name_ = "";
    node.left_ = nullptr;
    node.right_ = nullptr;
  }

public:
  void setValue(int value) {value_ = value;}
  int32_t getValue() {return value_;}

  void setName(std::string name) {name_ = name; }
  std::string getName() { return name_; }

  void setLeft(Node* left) { left_ = left; }
  void setRight(Node* right) { right_ = right; }
  void setNext(Node* left, Node* right) { left_ = left, right_ = right;}

  Node* getLeft() { return left_; }
  Node* getRight() { return right_; }

private:
  int32_t value_ = 0;
  std::string name_;
  Node* left_ = nullptr;
  Node* right_ = nullptr;
};

class Graph {
public:
  Graph() = default;
  void makeEmpty() { root_ = nullptr; }
  bool isEmpty() { root_ == nullptr; }
  bool insertNode(std::string name, int32_t value) {
    if (nodes_.find(name) != nodes_.end()) {
      std::cout << "node: " << name << "already exist, and failed to insert !!!" << std::endl;
      return false;
    }

    nodes_.insert({name, std::make_unique<Node>(name, value)});
    return true;
  }

  bool connectLeft(std::string src, std::string dst) {
    auto src_pair = nodes_.find(src);
    auto dst_pair = nodes_.find(dst);
    if (src_pair == nodes_.end()) {
      std::cout << "FAILED to connect for not find node: " << src << std::endl;
      return false;
    }

    if (dst_pair == nodes_.end()) {
      std::cout << "FAILED to connect for not find node: " << dst << std::endl;
      return false;
    }

    src_pair->second->setLeft(dst_pair->second.get());
  }

  bool connectRight(std::string src, std::string dst) {
    auto src_pair = nodes_.find(src);
    auto dst_pair = nodes_.find(dst);
    if (src_pair == nodes_.end()) {
      std::cout << "FAILED to connect for not find node: " << src << std::endl;
      return false;
    }

    if (dst_pair == nodes_.end()) {
      std::cout << "FAILED to connect for not find node: " << dst << std::endl;
      return false;
    }

    src_pair->second->setRight(dst_pair->second.get());
  }

  bool connectNext(std::string src, std::pair<std::string, std::string> dst) {
    auto src_pair = nodes_.find(src);
    const auto dst_left_pair = nodes_.find(dst.first);
    const auto dst_right_pair = nodes_.find(dst.second);
    if (src_pair == nodes_.end()) {
      std::cout << "FAILED to connect for not find node: " << src << std::endl;
      return false;
    }

    if (dst_left_pair == nodes_.end()) {
      std::cout << "FAILED to connect for not find node: " << dst.first << std::endl;
      return false;
    }

    if (dst_right_pair == nodes_.end()) {
      std::cout << "FAILED to connect for not find node: " << dst.second << std::endl;
      return false;
    }

    src_pair->second->setNext(dst_left_pair->second.get(), dst_right_pair->second.get());
  }

  bool setRoot(std::string name) {
    const auto& iter = nodes_.find(name);
    if (iter == nodes_.end()) {
      std::cout << "FAILED to find node: " << name << std::endl;
      return false;
    }
    root_ = iter->second.get();
    return true;
  }

  Node* getRoot() {
    return root_;
  }

  void printNodes() {
    for (const auto& item : nodes_) {
      std::cout << "node name: " << item.first << " : {";
      if (item.second->getLeft()) {
        std::cout << item.second->getLeft()->getName();
      }

      std::cout << ",";

      if (item.second->getRight()) {
        std::cout << item.second->getRight()->getName();
      }
      std::cout << "}" << std::endl;
    }
  }

public:
  // 前序遍历：根结点 ---> 左子树 ---> 右子树
  void preorder_traversal_recursive() {
    preorder_traversal_recursive(root_);
  }
  void preorder_traversal_recursive(Node* node) {
    if (node != nullptr) {
      std::cout << "node: " << node->getName() << " ==> ";
      std::cout << "value: " << node->getValue() << std::endl;
      preorder_traversal_recursive(node->getLeft());
      preorder_traversal_recursive(node->getRight());
    }
  }

  void preorder_traversal_stack() {
    preorder_traversal_stack(root_);
  }

  // void preorder_traversal_stack(Node* node) {
  //   std::stack<Node*> node_stack;
  //   if (!node) {
  //     std::cout << "node ptr is nullptr !!!" << std::endl;
  //   }

  //   node_stack.push(node);

  //   while (!node_stack.empty()) {
  //     auto node = node_stack.top();
  //     std::cout << "node: " << node->getName() << " ==> ";
  //     std::cout << "value: " << node->getValue() << std::endl;
  //     node_stack.pop();

  //     if (node->getRight()) {
  //       node_stack.push(node->getRight());
  //     }

  //     if (node->getLeft()) {
  //       node_stack.push(node->getLeft());
  //     }
  //   }
  // }
  void preorder_traversal_stack(Node* node) {
    std::stack<Node*> node_stack;
    while(!node_stack.empty() || node != nullptr) {
      if (node != nullptr) { //
        // 直接将根节点先输出
        std::cout << "node: " << node->getName() << " ==> ";
        std::cout << "value: " << node->getValue() << std::endl;
        node_stack.push(node);  // 首次根节点入栈
        node = node->getLeft(); // 每次都首先访问左节点
      } else {
        node = node_stack.top(); // node为空时再访问右节点
        node_stack.pop();        // 出栈, 改节点的左右子节点都访问了,之后不会再用了
        node = node->getRight(); // 重新赋值--> 相当于删除了栈中一个节点,
      }
    }
  }

  // 中序遍历：左子树 ---> 根节点 --> 右子树
  // 策略相似

private:
  std::unordered_map<std::string, std::unique_ptr<Node>> nodes_;
  Node* root_ = nullptr;
};

int main(int32_t argc, char* argv[]) {
  auto graph = std::make_unique<Graph>();
  graph->insertNode("a", 10);
  graph->insertNode("b", 20);
  graph->insertNode("c", 30);
  graph->insertNode("d", 40);
  graph->connectNext("a", {"b", "c"});
  graph->connectRight("b", "d");
  graph->setRoot("a");
  Node* root = graph->getRoot();
  // graph->preorder_traversal_recursive();
  graph->preorder_traversal_stack();
  std::cout << "run directed_graph_traverse.cpp successfully !!!" << std::endl;
  return 0;
}