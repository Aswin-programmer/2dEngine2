#pragma once

struct UserData;

// Custom comparator for std::pair<b2BodyId, b2BodyId>
struct BodyPairComparator
{
	bool operator()( const std::pair<b2BodyId, b2BodyId>& a, const std::pair<b2BodyId, b2BodyId>& b ) const
	{
		// Compare first body indices
		if ( a.first.index1 != b.first.index1 )
		{
			return a.first.index1 < b.first.index1;
		}
		// If first bodies are equal, compare second body indices
		return a.second.index1 < b.second.index1;
	}
};

class SensorListener
{
private:
	UserData* m_pUserSensorDataA{ nullptr };
	UserData* m_pUserSensorDataB{ nullptr };

	void SetUserSensorContacts( UserData* a, UserData* b );

public:
	void ProcessSensorContactEvents( b2WorldId myWorldId );

	// void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
	// void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;

	UserData* GetUserSensorDataA() { return m_pUserSensorDataA; }
	UserData* GetUserSensorDataB() { return m_pUserSensorDataB; }

	void ResetUserSensorAB();

	static std::map<std::pair<b2BodyId, b2BodyId>, std::pair<UserData*, UserData*>, BodyPairComparator> sensorData;
};
