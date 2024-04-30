import style from "./Home.module.css"
import Header from "../../components/Header/Header.jsx";
import Button_Route from '../../components/Button_Route.jsx';
import { SlArrowDown, SlArrowUp } from "react-icons/sl";
import { useRef } from "react";

/*
Pagina inicial do Projeto (HomePage)
*/
function Home() {

    const section1 = useRef(null);
    const section2 = useRef(null);
    const section3 = useRef(null);

    const scrollToSection = (sectionNumber) => {
        switch (sectionNumber) {
          case 1:
            section1.current.scrollIntoView({ behavior: 'smooth' });
            break;
          case 2:
            section2.current.scrollIntoView({ behavior: 'smooth' });
            break;
          case 3:
            section3.current.scrollIntoView({ behavior: 'smooth' });
            break;
          default:
            break;
        }
      };

    return (
        <>
            <div className="body">
                <Header />
                <section ref= {section1}>
                    <div className={style.section1}>
                        <h1 className={style.titulo}>START <br /> EXPLORING</h1>
                        <button className={style.Arrow}>
                                <SlArrowDown onClick={() => scrollToSection(2)}/>
                        </button>
                    </div>
                </section>
                <section ref= {section2}>
                    <div className={style.section2}>
                        <div className={style["menu-section2"]}>
                            <div className={style["menu-section2-conteudo1"]}/>
                            <div className={style["menu-section2-conteudo2"]}/>
                            <div className={style["menu-section2-conteudo3"]}/>
                        </div>
                        <div className= {style["sliders_big"]}>
                            <button className={style.slider1B}/>
                            <button className={style.slider2B}/>
                        </div>
                        <div className= {style["sliders_small"]}>
                            <button className={style.slider1S}/>
                            <button className={style.slider2S}/>
                            <button className={style.slider3S}/>
                            <button className={style.slider4S}/>
                        </div>
                    </div>
                </section>
                <section ref= {section3}>
                    <div className={style.section3}>
                        <button className={style.Arrow}>
                                <SlArrowUp onClick={() => scrollToSection(1)}/>
                        </button>
                    </div>
                </section>

            </div>
        </>
    );
}


export default Home