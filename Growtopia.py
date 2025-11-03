import streamlit as st

st.set_page_config(page_title="Growtopia | Smart Watering System", layout="wide")

# Google analytics
st.markdown(
    """
    <!-- Google tag (gtag.js) -->
    <script async src="https://www.googletagmanager.com/gtag/js?id=G-VCWKZ7L7B4"></script>
    <script>
      window.dataLayer = window.dataLayer || [];
      function gtag(){dataLayer.push(arguments);}
      gtag('js', new Date());
      gtag('config', 'G-VCWKZ7L7B4');
    </script>
    """,
    unsafe_allow_html=True
)

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
    max-width: 850px;
    margin: auto;
    text-align: center;
    color: #1b4332;
}

h1 {
    color: #1b4332 !important;
    font-weight: 700;
}
h2, h3, p, label {
    color: #2d6a4f !important;
}

img {
    border-radius: 15px;
    max-width: 80%;
    height: auto;
    margin-top: 1rem;
    margin-bottom: 1.5rem;
}
</style>
"""
st.markdown(page_style, unsafe_allow_html=True)

# main
st.markdown('<div class="main-container">', unsafe_allow_html=True)

st.title("🌿 Smart Watering for a Greener Tomorrow")
st.subheader("Automate your plant care with Growtopia — the IoT-powered system that saves water and keeps your plants thriving.")

# Landing Image
st.markdown(
    """
    <div style='text-align: center;'>
        <img src='https://i.imgur.com/GakGhFP.jpeg' width='550' style='border-radius:15px;'>
        <p style='color:#2d6a4f;'><i>Growtopia Smart Watering System</i></p>
    </div>
    """,
    unsafe_allow_html=True
)

# why growtopia
st.markdown("## 💧 Why Choose Growtopia?")

col1, col2, col3 = st.columns(3)
with col1:
    st.markdown("### 💦 Water Efficient")
    st.write("Reduces water use by up to **50%** while keeping your plants healthy.")
with col2:
    st.markdown("### 📱 Remote Monitoring")
    st.write("Control and track everything straight from your **phone or tablet**.")
with col3:
    st.markdown("### 🌱 Eco-Friendly")
    st.write("Supports **sustainable gardening** and responsible water use.")

# mission
st.markdown("## ☀️ Join Our Mission")
st.write("""
Growtopia isn’t just about technology — it’s about nurturing a movement.  
A movement that helps Filipinos grow smarter, conserve water, and connect with nature in meaningful ways.  
Together, we can make sustainable gardening a way of life for every household.
""")

st.caption("© 2025 Growtopia | Making gardening an enjoyable and sustainable activity for everyone.")

st.markdown('</div>', unsafe_allow_html=True)


