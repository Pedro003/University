import style from "./Events.module.css"
import React, { useState } from "react";
import Header from "../../components/Header/Header.jsx";
import Eventscard from "../../components/Eventscard/Eventscard.jsx";
import Modal from "react-modal";
import Calendar from 'react-calendar';
import 'react-calendar/dist/Calendar.css';

const categoriaslista = [
  <Eventscard evento="Maratona" categoria="Desporto" />,
  <Eventscard evento="Tempo" categoria="Cultura" />,
  <Eventscard evento="Xau" categoria="Educacao" />,
  <Eventscard evento="Passaros" categoria="Fotografia" />,
  <Eventscard evento="Brocas" categoria="Lazer" />,
  <Eventscard evento="Bom Jesus" categoria="Turismo" />,
];

Modal.setAppElement("#root");

function Events() {
  const [searchCat, setSearchCat] = useState('');
  const [selectedDate, setSelectedDate] = useState(new Date());
  const [modalOpen, setModalOpen] = useState(false);

  const handleChangeCat = (event) => {
    setSearchCat(event.target.value);
  };

  const handleDateChange = (date) => {
    setSelectedDate(date);
  };

  const openModal = () => {
    setModalOpen(true);
  };

  const closeModal = () => {
    setModalOpen(false);
  };

  const showAllCategories = searchCat === "";

  const categoriasFiltradas = categoriaslista.filter(evento => {
    const categoriaMatch = showAllCategories || evento.props.categoria === searchCat;
    const dataMatch = evento.props.data.toDateString() === selectedDate.toDateString();
    return categoriaMatch && dataMatch;
  });

  const categoriasFiltradasJSX = categoriasFiltradas.map((evento, index) => (
    <div key={index}>{evento}</div>
  ));

  return (
    <div className="body">
      <Header />
      <div className={style.events}>
        <h1 style={{ fontFamily: 'Arial, sans-serif', fontSize: '60px', padding: '35px', marginBottom: '5px' }}>Events</h1>

        <p className={style["button-text"]} onClick={openModal}> Abrir Filtros </p>

          <Modal
            isOpen={modalOpen} 
            onRequestClose={closeModal}
            className="modal"
            overlayClassName="modal-overlay"
            >

            <span className={style["close-button"]} onClick={closeModal}>&times;</span>
            
            <div className={style["search-container"]}>
              <select value={searchCat} onChange={handleChangeCat} className={style.caixaevents}>
                <option value="">Escolha uma categoria</option>
                <option value="Cultura">Cultura</option>
                <option value="Desporto">Desporto</option>
                <option value="Educacao">Educação</option>
                <option value="Fotografia">Fotografia</option>
                <option value="Lazer">Lazer</option>
                <option value="Turismo">Turismo</option>
              </select>
              <Calendar
                onChange={handleDateChange}
                value={selectedDate}
                minDate={new Date()}
                formatShortWeekday={(locale, date) => {
                  const weekdays = ['D', 'S', 'T', 'Q', 'Q', 'S', 'S'];
                  return weekdays[date.getDay()];
                }}
                className={style["calendarioE"]}
              />
            </div>
          </Modal>
        
        <div>{categoriasFiltradasJSX}</div>
      </div>
    </div>
  );
}

export default Events;
