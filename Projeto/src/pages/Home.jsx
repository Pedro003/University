import Header from "../components/Header.jsx";
import Button_Route from '../components/Button_Route.jsx';

function Home(){
    return(
        <>
            <Header/>
            <div className="imagens"/>
            <div className='menu-titulo'>
                <h1 className='titulo'>START</h1>
                <h1 className='titulo'>EXPLORING</h1>
            </div>   
            <div className="C1">
                <Button_Route page="/events" class_button="button1" class_button_name="button-word" name="Events"/>
                <Button_Route page="/locations" class_button="button2" class_button_name="button-word" name="locations"/>
            </div>
        </>
    );
}




export default Home