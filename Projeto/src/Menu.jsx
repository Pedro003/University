import Logo from './assets/Final.png'
import Switch from "./Switch.jsx"
import Data from "./Data.jsx"

function Menu(){
    return(
        <>
            <div className="header-line">
                <img className="logo" src={Logo} alt="profile picture"></img>
                <Switch/>
                <Data/>
                <span className="menu-links">
                    <a href="#">ABOUT US</a>
                    <a href="#">LOG IN</a>
                </span>
            </div>
            <hr className="upper-line"/>
            <div className="imagens"/>
            <div className='menu-titulo'>
                <h1 className='titulo'>START</h1>
                <h1 className='titulo'>EXPLORING</h1>
            </div>   
            <div className="C1">
                <button className='button1'>
                    <p className='button-word'>EVENTS</p>
                </button>
                <button className='button2'>
                    <p className='button-word'>LOCATIONS</p>
                </button>
            </div>
        </>
    );
}




export default Menu