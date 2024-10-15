#pragma once

#include <rlbot/Bot.h>
#include <rlbot/RLBotCPP.h>

#include <rlbot_generated.h>

#include <memory>

namespace rlbot
{
namespace detail
{
class BotManagerImpl;
}

/// @brief Bot manager base class
/// This should only be derived by the BotManager template class below
class RLBotCPP_API BotManagerBase
{
public:
	virtual ~BotManagerBase () noexcept;

	/// @brief Run bot manager
	/// @param host_ RLBotServer address
	/// @param port_ RLBotServer port
	/// @param agentId_ Agent id for connection
	/// @return Whether bot manager was successfully started
	bool run (char const *host_, char const *port_, char const *agentId_) noexcept;

	/// @brief Request bot manager to terminate
	void terminate () noexcept;

	/// @brief Start match
	/// @param matchSettings_ Match settings
	/// @note Uses this manager's agent to start a match
	void startMatch (rlbot::flat::MatchSettingsT const &matchSettings_) noexcept;

	/// @brief Start match
	/// @param host_ RLBotServer address
	/// @param port_ RLBotServer port
	/// @param matchSettings_ Match settings
	/// @note Uses a temporary agent to start a match
	static bool startMatch (char const *host_,
	    char const *port_,
	    rlbot::flat::MatchSettingsT const &matchSettings_) noexcept;

protected:
	/// @brief Parameterized constructor
	/// @param spawn_ Bot spawning function
	BotManagerBase (std::unique_ptr<Bot> (&spawn_) (int, int, std::string) noexcept) noexcept;

	/// @brief Bot manager implementation
	std::unique_ptr<detail::BotManagerImpl> m_impl;
};

/// @brief Bot manager
/// @tparam T Bot type
template <typename T>
    requires std::is_base_of_v<Bot, T>
class BotManager final : public BotManagerBase
{
public:
	BotManager () noexcept : BotManagerBase (BotManager::spawn)
	{
	}

private:
	/// @brief Bot spawning function
	/// @param index_ Index into gameTickPacket->players ()
	/// @param team_ Team (0 = Blue, 1 = Orange)
	/// @param name_ Bot name
	static std::unique_ptr<Bot>
	    spawn (int const index_, int const team_, std::string name_) noexcept
	{
		return std::make_unique<T> (index_, team_, std::move (name_));
	}
};
}