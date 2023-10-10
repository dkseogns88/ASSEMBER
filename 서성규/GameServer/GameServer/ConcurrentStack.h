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
		// - 한 방에 해결
		// -- 그런데 pop을 하면서 한번에 데이터를 뽑아올 수 있지 않을까??
		// -- 그렇게 안하는 이유는 데이터를 꺼내오는 순간에 crash가 생길 수가 있어서이다
		value = std::move(_stack.top());
		_stack.pop();
		return true;
	}

	void WaitPop(T& value) {
		unique_lock<mutex> lock(_mutex); 
		// - unique_lock을 사용하는 이유는 내부적으로 락을 풀어야 하는 경우가 발생해서
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
//		node->next = std::atomic_load(&_head); // 원자적으로 해야한다
//		
//		while(std::atomic_compare_exchange_weak(&_head, &node->next, node) == false)
//		{} // 밑에하고 같은 것
//		/*while (_head.compare_exchange_weak(node->next, node) == false)
//		{}*/
//
//		
//	}
//
//	shared_ptr<T> TryPop() {
//		shared_ptr<Node> oldHead = std::atomic_load(&_head); // 원자적으로 해야한다
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
		int32 externalCount = 0; // 참조권
		Node* ptr = nullptr;
	};

	struct Node {
		Node(const T& value) : data(make_shared<T>(value))
		{

		}

		shared_ptr<T> data;
		atomic<int32> internalCount = 0; // 참조 하는 애가 한명만 남았는지 확인
		CounterNodePtr next;
	};

public:

	void Push(const T& value) {

		CounterNodePtr node;
		node.ptr = new Node(value);
		node.externalCount = 1;
		// [! 다른 누군가가 끼어들 수 있음 -> 원자적으로 하자]
		node.ptr->next = _head;
		while (_head.compare_exchange_weak(node.ptr->next, node) == false)
		{
		}

	}

	shared_ptr<T> TryPop() {
		CounterNodePtr oldHead = _head;
		while (true) {
			
			// 참조권 획득 (externalCount를 현 시점 기준 +1 한 애가 이김)
			IncreaseHeadCount(oldHead);
			// 최소한 externalCount >=2 일테니 삭제X (안전하게 접근할 수 있는)
			Node* ptr = oldHead.ptr;

			// 데이터 없음
			if (ptr == nullptr)
				return shared_ptr<T>();

			// 소유권 획득 (ptr->next로 head를 바꿔치기 한 애가 이김, 참조를 여러 명이 가질 경우 먼저 소유권 획득 시 이김)
			if (_head.compare_exchange_weak(oldHead, ptr->next)) {

				shared_ptr<T> res;
				res.swap(ptr->data);

				// 나 말고 또 누가 있는가?? 참조하는 애가 존재 하는가?
				const int32 countIncrease = oldHead.externalCount - 2;

				// 나만 가지고 있네? -> 삭제하자
				// - fetch_add은 더하기 전에 값을 리턴함
				// - ex) 1(원래 값) + 2 = 3(새로운 값) 이지만 리턴은 원래 값을 리턴
				if (ptr->internalCount.fetch_add(countIncrease) == -countIncrease)
					delete ptr;

				// 다른 애들도 참조를 한 상태에서 지원주면 문제가 생긴다!
				return res;
			}

			// 마지막 참조권을 가지고 있는 애가 지워준다!!
			else if (ptr->internalCount.fetch_sub(1) == 1) {
				// 참조권은 얻었으나, 소유권은 실패 -> 뒷수습은 내가 한다
				delete ptr;
			}
			
		}

	}

	void IncreaseHeadCount(CounterNodePtr& oldCounter) {
		while (true) {
			CounterNodePtr newCounter = oldCounter;
			newCounter.externalCount++;
			// 경합이 발생할 수 있음
			if (_head.compare_exchange_weak(oldCounter, newCounter)) {
				oldCounter.externalCount = newCounter.externalCount;
				break;
			}
		}
	}

private:
	atomic<CounterNodePtr> _head;

};