import streamlit as st

st.set_page_config(page_title="Features | Growtopia", layout="wide")

# style
page_style = """
<style>
[data-testid="stAppViewContainer"] {
    background-color: #e9f7ec;
    background-size: cover;
    background-position: center;
}

[data-testid="stSidebar"] {
    background-color: #d8f3dc;
    color: #1b4332;
}
[data-testid="stSidebar"] * {
    color: #1b4332 !important;
    font-weight: 600;
}

[data-testid="stHeader"] {
    background: rgba(0,0,0,0);
}

.main-container {
    background: rgba(255, 255, 255, 0.94);
    padding: 3rem;
    border-radius: 20px;
    box-shadow: 0 4px 20px rgba(0,0,0,0.1);
    max-width: 900px;
    margin: 4rem auto;
    text-align: center;
    color: #1b4332;
}

h1 {
    color: #1b4332 !important;
    font-weight: 800;
}
h2, h3, p, li {
    color: #2d6a4f !important;
}

img {
    border-radius: 15px;
    max-width: 85%;
    height: auto;
    margin-top: 1.5rem;
    margin-bottom: 1.5rem;
}
</style>
"""
st.markdown(page_style, unsafe_allow_html=True)

# container
st.markdown('<div class="main-container">', unsafe_allow_html=True)

# page
st.title("⚙️ Features & Technology")

st.write("""
- 💧 **Automatic Watering:** Adjusts irrigation based on soil moisture data.  
- ✋ **Manual Override:** Notifies you when plants need water if you prefer to water manually.  
- 📱 **Mobile Monitoring:** Control and check your plants remotely through the Growtopia app.  
- 📊 **Water Efficiency Analytics:** Track your water usage to live more sustainably.
""")

st.markdown(
    """
    <div style='text-align: center;'>
        <img src='https://i.imgur.com/XLhdpnj_d.webp?maxwidth=760&fidelity=grand' width='600' style='border-radius:15px;'>
             width='600' 
             style='border-radius:15px;'>
    </div>
    """,
    unsafe_allow_html=True
)



st.markdown('</div>', unsafe_allow_html=True)

st.caption("© 2025 Growtopia | Making gardening an enjoyable and sustainable activity for everyone.")

st.markdown('</div>', unsafe_allow_html=True)

