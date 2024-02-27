import Header from "../components/Header.jsx";
import Button_Route from '../components/Button_Route.jsx';
import { SlArrowDown,SlArrowUp } from "react-icons/sl";
import { useState } from "react";
import "../styles/HomePage.css"


/*
Pagina inicial do Projeto (HomePage)
*/
function Home(){
    const [Arrow, setArrow] = useState(false);

  const toggleArrow = () => {
    setArrow(!Arrow);
  };
    return(
        <>
            <section className="top">
                <Header/>
                <div className="imagens"/>
                <div className='menu-titulo'>
                    <h1 className='titulo'>START</h1>
                    <h1 className='titulo'>EXPLORING</h1>
                    <button className="Arrow" onClick={toggleArrow}>
                    {!Arrow && (<SlArrowDown/>)}
                    {Arrow &&(<SlArrowUp/>)} 
                    </button>
                    {Arrow && (
                        <div>
                            <Button_Route page="/events" class_button="button1" class_button_name="button-word" name="Events"/>
                            <Button_Route page="/locations" class_button="button2" class_button_name="button-word" name="Locations"/>
                        </div>
                    )}       
                </div>       
            </section>
        </>
    );
}




export default Home