#pragma once

struct UserData;
struct b2WorldId;

class ContactListener
{
private:
	UserData* m_pUserDataA{ nullptr };
	UserData* m_pUserDataB{ nullptr };

	void SetUserContacts( UserData* a, UserData* b );

public:
	void BeginContact( b2WorldId myWorldId );
	void EndContact( b2WorldId myWorldId );

	// void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
	// void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;

	UserData* GetUserDataA() { return m_pUserDataA; }
	UserData* GetUserDataB() { return m_pUserDataB; }

	void ResetUserContactsAB();
};
