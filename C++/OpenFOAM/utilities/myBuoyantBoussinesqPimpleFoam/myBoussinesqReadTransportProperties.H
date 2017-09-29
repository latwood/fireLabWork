    singlePhaseTransportModel laminarTransport(U, phi);

	// fake value just to get the right dimensions for kappatEff [m^2/s]
    dimensionedScalar fakeKappaEff(laminarTransport.lookup("fakeKappaEff"));

    // Thermal expansion coefficient [1/K]
    dimensionedScalar beta(laminarTransport.lookup("beta"));

    // Reference temperature [K]
    dimensionedScalar TRef(laminarTransport.lookup("TRef"));

    // Laminar Prandtl number
    dimensionedScalar Pr(laminarTransport.lookup("Pr"));

    // Turbulent Prandtl number
    dimensionedScalar Prt(laminarTransport.lookup("Prt"));
