import { BrowserRouter, Routes, Route } from 'react-router-dom'
import Home from './pages/Home.jsx'
import About from './pages/About.jsx'
import Log_In from './pages/Log_In.jsx'
import Events from './pages/Events.jsx'
import Locations from './pages/Locations.jsx'

/*
O componente principal que configura as rotas da aplicação utilizando React Router.
 */
function App() {
    // Renderiza o componente principal da aplicação
    return (
        <>
            {/* Configura o roteamento usando BrowserRouter */}
            <BrowserRouter>
                {/* Define as rotas da aplicação */}
                <Routes>
                    {/* Rota inicial (página inicial) */}
                    <Route index element={<Home />} />
                    {/* Rota para a página inicial */}
                    <Route path="/home" element={<Home />} />
                    {/* Rota para a página "About Us" */}
                    <Route path="/about" element={<About />} />
                    {/* Rota para a página de login */}
                    <Route path="/log-in" element={<Log_In />} />
                    {/* Rota para a página de eventos */}
                    <Route path="/events" element={<Events />} />
                    {/* Rota para a página de locais */}
                    <Route path="/locations" element={<Locations />} />
                </Routes>
            </BrowserRouter>
        </>
    );
}

export default App