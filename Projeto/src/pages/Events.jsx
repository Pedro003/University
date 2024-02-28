import React,{ useState } from "react";
import Header from "../components/Header";
import Eventscard from "../components/Eventscard/Eventscard.jsx";
/*
Vai conter informações dos eventos atuais na cidade de Braga (Afonso the builder)
*/

/* Esta classename não é body é header tb*/



const categoriaslista =[
  <Eventscard evento ="Maratona" categoria="Desporto"/>,
  <Eventscard evento ="Tempo" categoria="Cultura"/>,
  <Eventscard evento ="Xau" categoria="Educacao"/>,
  <Eventscard evento ="Passaros" categoria="Fotografia"/>,
  <Eventscard evento ="Brocas" categoria="Lazer"/>,
  <Eventscard evento ="Bom Jesus" categoria="Turismo"/>,
]


function Events(){
    const [searchCat, setSearchCat] = useState('');

    const handleChangeCat = (event) => {
      setSearchCat(event.target.value);
    };

    const showAllCategories = searchCat === "";

    const categoriasFiltradas = showAllCategories ? categoriaslista : categoriaslista.filter(evento => evento.props.categoria === searchCat);

    const categoriasFiltradasJSX = categoriasFiltradas.map((evento, index) => (
      <div key={index}>
        {evento} 
      </div>
    ));

    return (
      <>
      <div className="body"> 
          <Header/>
              <div className="events">
                <h1 style={{ fontFamily: 'Arial, sans-serif', fontSize: '60px', padding: '35px', marginBottom: '5px' }}>Events</h1>

                  
                <div className="search-container">
                 
                  <select value={searchCat} onChange={handleChangeCat} className="caixaevents">
                    <option value="">Escolha uma categoria</option>
                    <option value="Cultura">Cultura</option>
                    <option value="Desporto">Desporto</option>
                    <option value="Educacao">Educação</option>
                    <option value="Fotografia">Fotografia</option>
                    <option value="Lazer">Lazer</option>
                    <option value="Turismo">Turismo</option>
                  </select>
                </div>

                <div>
                  {categoriasFiltradasJSX}
                </div>

              </div>
      </div>
      </>
    );
}


export default Events