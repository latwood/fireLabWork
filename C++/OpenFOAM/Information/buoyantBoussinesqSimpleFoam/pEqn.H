{
    volScalarField rAU("rAU", 1.0/UEqn().A());
    surfaceScalarField rAUf("Dp", fvc::interpolate(rAU));

    volVectorField HbyA("HbyA", U);
    HbyA = rAU*UEqn().H();
    UEqn.clear();

    surfaceScalarField phig(-rAUf*ghf*fvc::snGrad(rhok)*mesh.magSf());

    surfaceScalarField phiHbyA
    (
        "phiHbyA",
        (fvc::interpolate(HbyA) & mesh.Sf())
    );

    adjustPhi(phiHbyA, U, p_rgh);

    phiHbyA += phig;

    while (simple.correctNonOrthogonal())
    {
        fvScalarMatrix p_rghEqn
        (
            fvm::laplacian(rAUf, p_rgh) == fvc::div(phiHbyA)
        );

        p_rghEqn.setReference(pRefCell, getRefCellValue(p_rgh, pRefCell));

        p_rghEqn.solve();

        if (simple.finalNonOrthogonalIter())
        {
            // Calculate the conservative fluxes
            phi = phiHbyA - p_rghEqn.flux();

            // Explicitly relax pressure for momentum corrector
            p_rgh.relax();

            // Correct the momentum source with the pressure gradient flux
            // calculated from the relaxed pressure
            U = HbyA + rAU*fvc::reconstruct((phig - p_rghEqn.flux())/rAUf);
            U.correctBoundaryConditions();
        }
    }

    #include "continuityErrs.H"

    p = p_rgh + rhok*gh;
//if height = 30
//for rhok = 1, rhok*gh = 294
//for rhok = 0.955, rhok*gh = 280.77
//using bernouli eqn, v2 = sqrt(2*(p1-p2)/rhok)
//so for rhok = 1, v2 = 24.25
//for rhok = 0.955, v2 = same. This could explain the max velocity before relaxation
//for mesh, dzmax = 3000, use this as height
//so for rhok = 1, rhok*g*dzmax = 29400
//for rhok = 0.955, rhok*g*dzmax = 28077
//if these are pascals, this is equivalent to 0.27709845-0.29015544 atm, so 0.71 to 0.723 atm at the top of the domain

    if (p_rgh.needReference())
    {
        p += dimensionedScalar
        (
            "p",
            p.dimensions(),
            pRefValue - getRefCellValue(p, pRefCell)
        );
        p_rgh = p - rhok*gh;
    }
}
