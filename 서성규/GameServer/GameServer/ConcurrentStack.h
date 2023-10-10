#pragma once

#include <mutex>

template<class T>
class LockStack
{
public:
	LockStack(){}

	LockStack(const LockStack&) = delete;
	LockStack& operator=(const LockStack&) = delete;
	
	void Push(T& value) {
		lock_guard<mutex> lock(_mutex);
		_stack.push(std::move(value));
		_condVar.notify_one();
	}

	bool TryPop(T& value) {
		lock_guard<mutex> lock(_mutex);
		if (_stack.empty())
			return false;

		// empty -> top -> pop
		// - �� �濡 �ذ�
		// -- �׷��� pop�� �ϸ鼭 �ѹ��� �����͸� �̾ƿ� �� ���� ������??
		// -- �׷��� ���ϴ� ������ �����͸� �������� ������ crash�� ���� ���� �־�̴�
		value = std::move(_stack.top());
		_stack.pop();
		return true;
	}

	void WaitPop(T& value) {
		unique_lock<mutex> lock(_mutex); 
		// - unique_lock�� ����ϴ� ������ ���������� ���� Ǯ��� �ϴ� ��찡 �߻��ؼ�
		_condVar.wait(lock, [this] {return _stack.empty() == false; });
		value = std::move(_stack.top());
		_stack.pop();
	}

private:
	stack<T>			_stack;
	mutex				_mutex;
	condition_variable	_condVar;
};

//template<class T>
//class LockFreeStack {
//
//	struct Node {
//		Node(const T& value) : data(make_shared<T>(value)), next(nullptr)
//		{
//
//		}
//		
//		shared_ptr<T> data;
//		shared_ptr<Node> next;
//	};
//
//public:
//
//	void Push(const T& value) {
//		shared_ptr<Node> node = make_shared<Node>(value);
//		node->next = std::atomic_load(&_head); // ���������� �ؾ��Ѵ�
//		
//		while(std::atomic_compare_exchange_weak(&_head, &node->next, node) == false)
//		{} // �ؿ��ϰ� ���� ��
//		/*while (_head.compare_exchange_weak(node->next, node) == false)
//		{}*/
//
//		
//	}
//
//	shared_ptr<T> TryPop() {
//		shared_ptr<Node> oldHead = std::atomic_load(&_head); // ���������� �ؾ��Ѵ�
//
//		while (oldHead && std::atomic_compare_exchange_weak(&_head, &oldHead, oldHead->next) == false)
//		{}
//		
//		if (oldHead == nullptr)
//			return shared_ptr<T>();
//
//
//		return oldHead->data;
//	}
//
//private:
//	shared_ptr<Node> _head;
//
//};


template<class T>
class LockFreeStack {

	struct Node;

	struct CounterNodePtr {
		int32 externalCount = 0; // ������
		Node* ptr = nullptr;
	};

	struct Node {
		Node(const T& value) : data(make_shared<T>(value))
		{

		}

		shared_ptr<T> data;
		atomic<int32> internalCount = 0; // ���� �ϴ� �ְ� �Ѹ� ���Ҵ��� Ȯ��
		CounterNodePtr next;
	};

public:

	void Push(const T& value) {

		CounterNodePtr node;
		node.ptr = new Node(value);
		node.externalCount = 1;
		// [! �ٸ� �������� ����� �� ���� -> ���������� ����]
		node.ptr->next = _head;
		while (_head.compare_exchange_weak(node.ptr->next, node) == false)
		{
		}

	}

	shared_ptr<T> TryPop() {
		CounterNodePtr oldHead = _head;
		while (true) {
			
			// ������ ȹ�� (externalCount�� �� ���� ���� +1 �� �ְ� �̱�)
			IncreaseHeadCount(oldHead);
			// �ּ��� externalCount >=2 ���״� ����X (�����ϰ� ������ �� �ִ�)
			Node* ptr = oldHead.ptr;

			// ������ ����
			if (ptr == nullptr)
				return shared_ptr<T>();

			// ������ ȹ�� (ptr->next�� head�� �ٲ�ġ�� �� �ְ� �̱�, ������ ���� ���� ���� ��� ���� ������ ȹ�� �� �̱�)
			if (_head.compare_exchange_weak(oldHead, ptr->next)) {

				shared_ptr<T> res;
				res.swap(ptr->data);

				// �� ���� �� ���� �ִ°�?? �����ϴ� �ְ� ���� �ϴ°�?
				const int32 countIncrease = oldHead.externalCount - 2;

				// ���� ������ �ֳ�? -> ��������
				// - fetch_add�� ���ϱ� ���� ���� ������
				// - ex) 1(���� ��) + 2 = 3(���ο� ��) ������ ������ ���� ���� ����
				if (ptr->internalCount.fetch_add(countIncrease) == -countIncrease)
					delete ptr;

				// �ٸ� �ֵ鵵 ������ �� ���¿��� �����ָ� ������ �����!
				return res;
			}

			// ������ �������� ������ �ִ� �ְ� �����ش�!!
			else if (ptr->internalCount.fetch_sub(1) == 1) {
				// �������� �������, �������� ���� -> �޼����� ���� �Ѵ�
				delete ptr;
			}
			
		}

	}

	void IncreaseHeadCount(CounterNodePtr& oldCounter) {
		while (true) {
			CounterNodePtr newCounter = oldCounter;
			newCounter.externalCount++;
			// ������ �߻��� �� ����
			if (_head.compare_exchange_weak(oldCounter, newCounter)) {
				oldCounter.externalCount = newCounter.externalCount;
				break;
			}
		}
	}

private:
	atomic<CounterNodePtr> _head;

};