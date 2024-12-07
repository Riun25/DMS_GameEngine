#include "framework.h"



template <typename T>
class Serializable
{
public:
	void Serialize(const T& _obj, const std::string& _file)
	{
		std::ofstream ofs(_file, std::ios::binary);

		assert(ofs, "file not found");

		auto fields = _obj.to_tuple();
		auto sequence = std::make_index_sequence<std::tuple_size_v<decltype(fields)>>{};
		SerializeFields(ofs, fields, sequence);

		ofs.close();
	}

	std::shared_ptr<T> Deserialize(const std::string& fileName) {
		std::ifstream ifs(fileName, std::ios::binary);
		if (!ifs) {
			throw std::runtime_error("파일을 열 수 없습니다.");
		}

		std::shared_ptr<T> object = std::make_shared<T>;
		auto fields = object.to_tuple();
		auto sequence = std::make_index_sequence<std::tuple_size_v<decltype(fields)>>{};
		DeserializeFields(ifs, fields, sequence);

		ifs.close();

		object.from_tuple(fields); // 튜플에서 멤버 변수 복사

		return object;
	}

private:
	template <typename Tuple, std::size_t... Is>
	void SerializeFields(std::ofstream& ofs, const Tuple& t, std::index_sequence<Is...>)
	{
		(ofs.write(reinterpret_cast<const char*>(&std::get<Is>(t)), sizeof(std::get<Is>(t))), ...);
	}

	template <typename Tuple, std::size_t... Is>
	void DeserializeFields(std::ifstream& ifs, Tuple& t, std::index_sequence<Is...>)
	{
		(ifs.read(reinterpret_cast<char*>(&std::get<Is>(t)), sizeof(std::get<Is>(t))), ...);
	}

};
