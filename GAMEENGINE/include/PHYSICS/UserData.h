#pragma once

struct UserData
{
	std::any userData{};
	std::uint32_t type_id{ 0 };
};

struct ObjectData
{
	std::string tag{ "" }, group{ "" };
	bool bCollider{ false }, bTrigger{ false }, bSensor{ false };
	std::uint32_t entityID{};
	std::vector<const ObjectData*> contactEntities;

	friend bool operator==( const ObjectData& a, const ObjectData& b );
	[[nodiscard]] std::string to_string() const;

private:
	bool AddContact( const ObjectData* objectData );
	bool RemoveContact( const ObjectData* objectData );

	// The add/remove contact functions should never be called
	// outside of the contact listener.
	friend class ContactListener;
	friend class SensorListener;
};
