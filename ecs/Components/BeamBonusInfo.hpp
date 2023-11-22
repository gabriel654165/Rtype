//
// Created by spriters on 20/11/2021.
//

#ifndef B_CPP_501_PAR_5_1_RTYPE_MAXIME_DODIN_BEAMBONUSINFO_HPP
#define B_CPP_501_PAR_5_1_RTYPE_MAXIME_DODIN_BEAMBONUSINFO_HPP

struct BeamBonusInfo {
    std::size_t beamIndice{};
    std::size_t animationId;
};

struct BonusInfo {
    BonusInfo(BeamBonusInfo &t) : type(BEAM_BONUS),  infos(t) {};

    enum Type : size_t {
        BEAM_BONUS,
        HEALTH_BONUS,
        BONUS_COUNT
    } type;

    std::any infos;
};

#endif //B_CPP_501_PAR_5_1_RTYPE_MAXIME_DODIN_BEAMBONUSINFO_HPP
