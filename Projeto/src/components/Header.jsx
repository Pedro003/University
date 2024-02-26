import Button_Route from "./Button_Route";
import Switch from "./Switch";
import Data from "./Data";
import Temp from "./Temp";

/*
    Um componente de cabeçalho que contém elementos comuns de navegação e informações.
 */
function Header() {
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
            </div>

            {/* Linha horizontal separadora */}
            <hr className="upper-line"/>
        </>
    );
}

export default Header;