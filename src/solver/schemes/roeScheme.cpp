/* 
/Fichier roeScheme.cpp
/Date: 09 novembre 2020 - 04h33
/Projet: Projet Intégrateur 4
*/

#include "./roeScheme.hpp"

RoeScheme::RoeScheme(MeshData *meshData, InputData *inputData, Flux *F, Solution *W)
{
    _meshData = meshData;
    _inputData = inputData;
    _W = W;
    _F = F;

    _Fcentre->rhoV.assign(_meshData->getNFACE(), 0.);
    _Fcentre->rhouV.assign(_meshData->getNFACE(), 0.);
    _Fcentre->rhovV.assign(_meshData->getNFACE(), 0.);
    _Fcentre->rhoHV.assign(_meshData->getNFACE(), 0.);

    _Fdissip->rhoV.assign(_meshData->getNFACE(), 0.);
    _Fdissip->rhouV.assign(_meshData->getNFACE(), 0.);
    _Fdissip->rhovV.assign(_meshData->getNFACE(), 0.);
    _Fdissip->rhoHV.assign(_meshData->getNFACE(), 0.);

    _esuf = _meshData->getEsuf();
    _face2Normales = _meshData->getFace2Normales();

    return;
}

RoeScheme::~RoeScheme()
{
    return;
}

void RoeScheme::computeConvectivesFlux()
{
    return;
}

void RoeScheme::computeFluxCentres()
{
    int L, R;
    int R;
    double Vn_L, Vn_R;
    for (int iFace = 0; iFace < _meshData->getNFACE(); iFace++)
    {
        L = _esuf->at(2 * iFace + 0);
        R = _esuf->at(2 * iFace + 1);
        Vn_L = _W->U[L] * _face2Normales->at(2 * iFace + 0) + _W->V[L] * _face2Normales->at(2 * iFace + 1);
        Vn_R = _W->U[R] * _face2Normales->at(2 * iFace + 0) + _W->V[R] * _face2Normales->at(2 * iFace + 1);
        _Fcentre->rhoV[iFace] = 0.5 * (_W->rho[L] * Vn_L + _W->rho[R] * Vn_R);
        _Fcentre->rhouV[iFace] = 0.5 * (_W->rho[L] * _W->U[L] * Vn_L + _W->rho[R] * _W->U[R] * Vn_R + _face2Normales->at(iFace + 0) * (_W->p[L] + _W->p[R]));
        _Fcentre->rhovV[iFace] = 0.5 * (_W->rho[L] * _W->V[L] * Vn_L + _W->rho[R] * _W->V[R] * Vn_R + _face2Normales->at(iFace + 1) * (_W->p[L] + _W->p[R]));
        _Fcentre->rhoHV[iFace] = 0.5 * (_W->rho[L] * _W->H[L] * Vn_L + _W->rho[R] * _W->H[R] * Vn_R);
    }
    return;
}

void RoeScheme::computeFluxDissip()
{
    int L, R;
    int R;
    double Vn_L, Vn_R, rho_, u_, v_, H_, c_, q2_, V_;
    for (int iFace = 0; iFace < _meshData->getNFACE(); iFace++)
    {
        L = _esuf->at(2 * iFace + 0);
        R = _esuf->at(2 * iFace + 1);
        Vn_L = _W->U[L] * _face2Normales->at(2 * iFace + 0) + _W->V[L] * _face2Normales->at(2 * iFace + 1);
        Vn_R = _W->U[R] * _face2Normales->at(2 * iFace + 0) + _W->V[R] * _face2Normales->at(2 * iFace + 1);
        rho_ = sqrt(_W->rho[L] * _W->rho[R]);
        u_ = (_W->U[L] * sqrt(_W->rho[L]) + _W->U[R] * sqrt(_W->rho[R])) / (sqrt(_W->rho[L]) + sqrt(_W->rho[R]));
        v_ = (_W->V[L] * sqrt(_W->rho[L]) + _W->V[R] * sqrt(_W->rho[R])) / (sqrt(_W->rho[L]) + sqrt(_W->rho[R]));
        H_ = (_W->H[L] * sqrt(_W->rho[L]) + _W->H[R] * sqrt(_W->rho[R])) / (sqrt(_W->rho[L]) + sqrt(_W->rho[R]));
        q2_ = u_ * u_ + v_ * v_;
        c_ = sqrt((_inputData->getRatioCpCv() - 1) * (H_ - q2_ * 0.5));
        V_ = u_ * _face2Normales->at(2 * iFace + 0) + v_ * _face2Normales->at(2 * iFace + 1);
        _Fcentre->rhoV[iFace] = 0.5 * (_W->rho[L] * Vn_L + _W->rho[R] * Vn_R);
        _Fcentre->rhouV[iFace] = 0.5 * (_W->rho[L] * _W->U[L] * Vn_L + _W->rho[R] * _W->U[R] * Vn_R + _face2Normales->at(iFace + 0) * (_W->p[L] + _W->p[R]));
        _Fcentre->rhovV[iFace] = 0.5 * (_W->rho[L] * _W->V[L] * Vn_L + _W->rho[R] * _W->V[R] * Vn_R + _face2Normales->at(iFace + 1) * (_W->p[L] + _W->p[R]));
        _Fcentre->rhoHV[iFace] = 0.5 * (_W->rho[L] * _W->H[L] * Vn_L + _W->rho[R] * _W->H[R] * Vn_R);
    }
    return;
}
