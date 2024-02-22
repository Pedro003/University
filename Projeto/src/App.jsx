import { BrowserRouter, Routes, Route } from 'react-router-dom'
import Home from './pages/Home.jsx'
import About from './pages/About.jsx'
import Log_In from './pages/Log_In.jsx'
import Events from './pages/Events.jsx'
import Locations from './pages/Locations.jsx'

function App() {
    return(
      <>
      <BrowserRouter>
        <Routes>
          <Route index element = {<Home/>} />
          <Route path="/home" element = {<Home/>} />
          <Route path="/about" element = {<About/>} />
          <Route path="/log-in" element = {<Log_In/>} />
          <Route path="/events" element = {<Events/>} />
          <Route path="/locations" element = {<Locations />} />
        </Routes>
      </BrowserRouter>
      </>
    );
}

export default App
