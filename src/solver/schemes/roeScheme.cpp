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
    _Fcentre = new Flux();
    _Fdissip = new Flux();
    _Fcentre->rhoV.assign(_meshData->getNFACE() - _meshData->getNBOUNDARY(), 0.);
    _Fcentre->rhouV.assign(_meshData->getNFACE() - _meshData->getNBOUNDARY(), 0.);
    _Fcentre->rhovV.assign(_meshData->getNFACE() - _meshData->getNBOUNDARY(), 0.);
    _Fcentre->rhoHV.assign(_meshData->getNFACE() - _meshData->getNBOUNDARY(), 0.);

    _Fdissip->rhoV.assign(_meshData->getNFACE() - _meshData->getNBOUNDARY(), 0.);
    _Fdissip->rhouV.assign(_meshData->getNFACE() - _meshData->getNBOUNDARY(), 0.);
    _Fdissip->rhovV.assign(_meshData->getNFACE() - _meshData->getNBOUNDARY(), 0.);
    _Fdissip->rhoHV.assign(_meshData->getNFACE() - _meshData->getNBOUNDARY(), 0.);

    _esuf = _meshData->getEsuf();
    _face2Normales = _meshData->getFace2Normales();

    return;
}

RoeScheme::~RoeScheme()
{
    delete _Fcentre;
    delete _Fdissip;
    return;
}

void RoeScheme::computeConvectivesFlux()
{
    computeFluxCentres();
    computeFluxDissip();
    for (int iFace = 0; iFace < _meshData->getNFACE() - _meshData->getNBOUNDARY(); iFace++)
    {
        _F->rhoV[iFace] = _Fcentre->rhoV[iFace] - _Fdissip->rhoV[iFace];
        _F->rhouV[iFace] = _Fcentre->rhouV[iFace] - _Fdissip->rhovV[iFace];
        _F->rhovV[iFace] = _Fcentre->rhovV[iFace] - _Fdissip->rhovV[iFace];
        _F->rhoHV[iFace] = _Fcentre->rhoHV[iFace] - _Fdissip->rhoHV[iFace];
    }
    return;
}

void RoeScheme::computeFluxCentres()
{
    int L, R;
    double Vn_L, Vn_R;
    for (int iFace = 0; iFace < _meshData->getNFACE() - _meshData->getNBOUNDARY(); iFace++)
    {
        L = _esuf->at(2 * iFace + 0);
        R = _esuf->at(2 * iFace + 1);

        Vn_L = _W->rhoU[L] / _W->rho[L] * _face2Normales->at(2 * iFace + 0) + _W->rhoV[L] / _W->rho[L] * _face2Normales->at(2 * iFace + 1);
        Vn_R = _W->rhoU[R] / _W->rho[R] * _face2Normales->at(2 * iFace + 0) + _W->rhoV[R] / _W->rho[R] * _face2Normales->at(2 * iFace + 1);

        _Fcentre->rhoV[iFace] = 0.5 * (_W->rho[L] * Vn_L + _W->rho[R] * Vn_R);
        _Fcentre->rhouV[iFace] = 0.5 * (_W->rhoU[L] * Vn_L + _W->rhoU[R] * Vn_R + _face2Normales->at(2 * iFace + 0) * (_W->p[L] + _W->p[R]));
        _Fcentre->rhovV[iFace] = 0.5 * (_W->rhoV[L] * Vn_L + _W->rhoV[R] * Vn_R + _face2Normales->at(2 * iFace + 1) * (_W->p[L] + _W->p[R]));
        _Fcentre->rhoHV[iFace] = 0.5 * (_W->rho[L] * _W->H[L] * Vn_L + _W->rho[R] * _W->H[R] * Vn_R);
    }
    return;
}

void RoeScheme::computeFluxDissip()
{
    int L, R;
    double Vn_L, Vn_R, rho_, u_, v_, H_, c_, q2_, V_;
    double DF1, DF234, DF5;
    for (int iFace = 0; iFace < _meshData->getNFACE() - _meshData->getNBOUNDARY(); iFace++)
    {
        L = _esuf->at(2 * iFace + 0);
        R = _esuf->at(2 * iFace + 1);

        Vn_L = _W->rhoU[L] / _W->rho[L] * _face2Normales->at(2 * iFace + 0) + _W->rhoV[L] / _W->rho[L] * _face2Normales->at(2 * iFace + 1);
        Vn_R = _W->rhoU[R] / _W->rho[R] * _face2Normales->at(2 * iFace + 0) + _W->rhoV[R] / _W->rho[R] * _face2Normales->at(2 * iFace + 1);

        rho_ = sqrt(_W->rho[L] * _W->rho[R]);

        u_ = (_W->rhoU[L] / sqrt(_W->rho[L]) + _W->rhoU[R] / sqrt(_W->rho[R])) / (sqrt(_W->rho[L]) + sqrt(_W->rho[R]));
        v_ = (_W->rhoV[L] / sqrt(_W->rho[L]) + _W->rhoV[R] / sqrt(_W->rho[R])) / (sqrt(_W->rho[L]) + sqrt(_W->rho[R]));

        H_ = (_W->H[L] * sqrt(_W->rho[L]) + _W->H[R] * sqrt(_W->rho[R])) / (sqrt(_W->rho[L]) + sqrt(_W->rho[R]));

        q2_ = u_ * u_ + v_ * v_;
        c_ = sqrt((_inputData->getRatioCpCv() - 1) * (H_ - q2_ * 0.5));

        V_ = u_ * _face2Normales->at(2 * iFace + 0) + v_ * _face2Normales->at(2 * iFace + 1);

        DF1 = abs(V_ - c_) * ((_W->p[R] - _W->p[L] - rho_ * c_ * (Vn_R - Vn_L)) / (2 * c_ * c_));
        DF234 = ((_W->rho[R] - _W->rho[L]) - (_W->p[R] - _W->p[L]) / (c_ * c_));
        DF5 = abs(V_ + c_) * ((_W->p[R] - _W->p[L] + rho_ * c_ * (Vn_R - Vn_L)) / (2 * c_ * c_));

        _Fdissip->rhoV[iFace] = 0.5 * (DF1 * 1. + abs(V_) * (DF234 * 1. + 0.) + DF5 * 1.);

        _Fdissip->rhouV[iFace] = 0.5 * (DF1 * (u_ - c_ * _face2Normales->at(2 * iFace + 0)) + DF5 * (u_ + c_ * _face2Normales->at(2 * iFace + 0)) + abs(V_) * (DF234 * u_ + rho_ * (_W->rhoU[R] / _W->rho[R] - _W->rhoU[L] / _W->rho[L] - (Vn_R - Vn_L) * _face2Normales->at(2 * iFace + 0))));
        _Fdissip->rhovV[iFace] = 0.5 * (DF1 * (v_ - c_ * _face2Normales->at(2 * iFace + 1)) + DF5 * (v_ + c_ * _face2Normales->at(2 * iFace + 1)) + abs(V_) * (DF234 * v_ + rho_ * (_W->rhoV[R] / _W->rho[R] - _W->rhoV[L] / _W->rho[L] - (Vn_R - Vn_L) * _face2Normales->at(2 * iFace + 1))));

        _Fdissip->rhoHV[iFace] = 0.5 * (DF1 * (H_ - c_ * V_) + DF5 * (H_ + c_ * V_) + abs(V_) * (DF234 * q2_ * 0.5 + rho_ * (u_ * (_W->rhoU[R] / _W->rho[R] - _W->rhoU[L] / _W->rho[L]) + v_ * (_W->rhoV[R] / _W->rho[R] - _W->rhoV[L] / _W->rho[L]) - (Vn_R - Vn_L) * V_)));
    }
    return;
}
