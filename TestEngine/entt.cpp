#include <entt/entt.hpp>

struct Pos
{
	float x, y, z;
};

struct Rot
{
	float x, y, z;
};

struct Scl
{
	float x, y, z;
};

void Update(entt::registry& _registry)
{
	auto view = _registry.view<const Pos, Rot, Scl>();

	view.each([](const auto& pos, auto& rot, auto& scl) {});

	view.each([](const auto entity, auto& pos, auto& rot) {});

	for (auto [entity, pos, rot, scl] : view.each())
	{

	}

	for (auto entity : view)
	{
		auto& rot = view.get<Rot>(entity);
		auto& scl = view.get<Scl>(entity);
	}
}

int main()
{
	entt::registry _registry;

	for (auto i = 0u; i < 10u; i++)
	{
		const auto _entity = _registry.create();
		_registry.emplace<Pos>(_entity, i * 1.f, i * 1.f, i * 1.f);
		if (i % 2 == 0)
		{
			_registry.emplace<Rot>(_entity, i * 1.f, i * 1.f, i * 1.f);
		}
	}



	return 0;
}