#pragma once

#include <assert.h>

#include <windows.h>

#include <iostream>

#include <string>
#include <thread>
#include <mutex>



class SingletonFinalizer
{
public  :
	typedef void(*FinalizerFunc)();

	static void AddFinalizer(FinalizerFunc func);
	static void Finalize();

};

//Singleton�N���X�̃e���v���[�g
//�C���X�^���X����ȏ����Ȃ����Ƃ�ۏ�

template <class T>
class Singleton
{
public :
	
	//�C���X�^���X�̎擾
	static T& GetInstance()
	{
		std::call_once(initFlag, Create);
		assert(pInstance);
		return *pInstance;
	}
	
protected:


	Singleton()
	{
	/*	char szBuff[256];
		wsprintf(szBuff, "SINGLETON template %s",typeid(*this).name());
		std::string st = szBuff;
		MessageBox(NULL, st.c_str(), TEXT("�쐬"), MB_OK);*/
	}

	virtual ~Singleton()
	{
		/*char szBuff[256];
		wsprintf(szBuff, "SINGLETON template %s", typeid(*this).name());
		std::string st = szBuff;
		MessageBox(NULL, st.c_str(), TEXT("�J��"), MB_OK);*/
	}//�f�X�g���N�^

	//
	static bool IsInstatnceCreated(){ return (pInstance != nullptr); }

private:
	//�C���X�^���X���w���|�C���^
	static T* pInstance;

	static std::once_flag initFlag;

	//�쐬�֐�
	static  void Create()
	{
		pInstance = new T;
		SingletonFinalizer::AddFinalizer(&Singleton<T>::Releace);
	}

	//�J���֐�
	static void Releace()
	{
		delete pInstance;
		pInstance = nullptr;
	}

	//������Z�ƃR�s�[�R���X�g���N�^��private�ɂ��ĊO��������s�ł��Ȃ�����
	void operator =(const Singleton& obj){}
	Singleton(const Singleton &obj){}

	
};

template <class T>std::once_flag Singleton<T>::initFlag;
template <class T>T* Singleton<T>::pInstance = nullptr;