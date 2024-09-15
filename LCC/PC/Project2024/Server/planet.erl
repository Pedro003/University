-module(planet).
-export([start/1]).

-record(planet, {orbit_radius, orbit_speed, radius, angle = rand:uniform() * 6.28, pos}).

-define(NUM_PLANETS, 5).
-define(SOL_POS, {625, 400}).

start(MatchPid) ->
    % Inicializar planetas
    Planets = [
        init(150 + I * 50, rand:uniform() * 0.05, 20)
     || I <- lists:seq(0, ?NUM_PLANETS - 1)
    ],
    %! comunicação com o processo principal da match
    MatchPid ! {planets, Planets},
    % Iniciar loop do jogo
    loop(Planets, MatchPid).

loop(Planets, MatchPid) ->
    % Atualizar e desenhar planetas
    UpdatedPlanets = [update(P) || P <- Planets],
    timer:sleep(50),
    %! comunicação com o processo principal da match
    MatchPid ! {planets, UpdatedPlanets},
    loop(UpdatedPlanets, MatchPid).

%% Função de Inicialização
init(OrbitRadius, OrbitSpeed, Radius) ->
    Angle = rand:uniform() * 2 * math:pi(),
    Pos = calc_position(625, 400, OrbitRadius, Angle),
    #planet{
        orbit_radius = OrbitRadius,
        orbit_speed = OrbitSpeed,
        radius = Radius,
        angle = Angle,
        pos = Pos
    }.

%% Função de Atualização
update(#planet{orbit_radius = OrbitRadius, orbit_speed = OrbitSpeed, angle = Angle} = Planet) ->
    NewAngle = Angle + OrbitSpeed,
    NewPos = calc_position(625, 400, OrbitRadius, NewAngle),
    Planet#planet{angle = NewAngle, pos = NewPos}.

%% Função Auxiliar para Cálculo da Posição
calc_position(SolX, SolY, OrbitRadius, Angle) ->
    {SolX + OrbitRadius * math:cos(Angle), SolY + OrbitRadius * math:sin(Angle)}.
