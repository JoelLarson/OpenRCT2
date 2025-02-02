/*****************************************************************************
 * Copyright (c) 2014-2020 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include "GuestSetFlagsAction.h"

#include "../Context.h"
#include "../OpenRCT2.h"

GuestSetFlagsAction::GuestSetFlagsAction(uint16_t peepId, uint32_t flags)
    : _peepId(peepId)
    , _newFlags(flags)
{
}

void GuestSetFlagsAction::AcceptParameters(GameActionParameterVisitor& visitor)
{
    visitor.Visit("peep", _peepId);
    visitor.Visit("flags", _newFlags);
}

uint16_t GuestSetFlagsAction::GetActionFlags() const
{
    return GameAction::GetActionFlags() | GameActions::Flags::AllowWhilePaused;
}

void GuestSetFlagsAction::Serialise(DataSerialiser& stream)
{
    GameAction::Serialise(stream);

    stream << DS_TAG(_peepId) << DS_TAG(_newFlags);
}

GameActions::Result::Ptr GuestSetFlagsAction::Query() const
{
    Peep* peep = TryGetEntity<Peep>(_peepId);
    if (peep == nullptr)
    {
        log_error("Used invalid sprite index for peep: %u", static_cast<uint32_t>(_peepId));
        return MakeResult(GameActions::Status::InvalidParameters, STR_CANT_CHANGE_THIS);
    }
    return std::make_unique<GameActions::Result>();
}

GameActions::Result::Ptr GuestSetFlagsAction::Execute() const
{
    Peep* peep = TryGetEntity<Peep>(_peepId);
    if (peep == nullptr)
    {
        log_error("Used invalid sprite index for peep: %u", static_cast<uint32_t>(_peepId));
        return MakeResult(GameActions::Status::InvalidParameters, STR_CANT_CHANGE_THIS);
    }

    peep->PeepFlags = _newFlags;

    return std::make_unique<GameActions::Result>();
}
