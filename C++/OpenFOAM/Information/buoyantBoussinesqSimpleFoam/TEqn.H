{
    kappat = turbulence->nut()/Prt;
    kappat.correctBoundaryConditions();

    volScalarField kappaEff("kappaEff", turbulence->nu()/Pr + kappat);

    fvScalarMatrix TEqn
    (
        fvm::div(phi, T)
      - fvm::laplacian(kappaEff, T)
    );

    TEqn.relax();
    TEqn.solve();

    rhok = 1.0 - beta*(T - TRef);
//1-beta(T-Tref) can't be done if T-Tref > 15
//beta = 0.003 for air at 300 K
//for T-Tref = 0, rhok = 1
//for T-Tref = 5, rhok = 0.985
//for T-Tref = 10, rhok = 0.97
//for T-Tref = 15, rhok = 0.955
}
