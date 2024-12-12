#pragma once
#include "IData.h"

/// <summary>
/// ������ ���� �� �����ϴ� �������� ���� Ŭ�����Դϴ�.
/// ������ ���� ������ ���� ���� �����͸� ��ȣȭ�Ͽ� �����մϴ�.
/// </summary>
class TestPlayerData : public IData
{
public:
    TestPlayerData() {}
    ~TestPlayerData() {}

    void Serialize();
    void Deserialize();

private:
    int mGold;
    int mWood;
    int mIron;
    float mLevel;

};

