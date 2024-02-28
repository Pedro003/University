import Button_Route from "./Button_Route";
import Switch from "./Switch";
import Data from "./Data";
import Temp from "./Temp";
import {CiMenuFries} from "react-icons/ci"
import { useState } from "react";

/*
    Um componente de cabeçalho que contém elementos comuns de navegação e informações.
 */
function Header() {
    const [estaAberto, setEstaAberto] = useState(false);

  const toggleAberto = () => {
    setEstaAberto(!estaAberto);
  };
    // Renderiza o componente do cabeçalho
    return (
        <>
            {/* Linha do cabeçalho */}
            <div className="header-line">
                {/* Botão de rota para a página inicial */}
                <Button_Route page="/home" class_button="home"/>

                {/* Exibição de data/hora e temperatura em tempo real */}
                <span className="menu-info">
                    <Data/>
                    <Temp name="temperatura"/>
                </span>

                {/* Links do menu */}
                <span className="menu-links">
                    <a href="/about">ABOUT US</a>
                    <a href="/log-in">LOG IN</a>
                </span>    
                <button className="menu-drop" onClick={toggleAberto}>
                    <CiMenuFries/> 
                </button>
                {estaAberto && (
                    <div className="menu-tabela">
                        <a href="/home">HOME</a>
                        <a href="/events">EVENTS</a>
                        <a href="/log-in">LOCATIONS</a>
                        <a href="/about">ABOUT US</a>
                        <a href="/log-in">LOG IN</a>
                    </div>
                )};
            </div>

            {/* Linha horizontal separadora */}
            <hr className="upper-line"/>
        </>
    );
}

export default Header;