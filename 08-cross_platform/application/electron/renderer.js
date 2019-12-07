// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// No Node.js APIs are available in this process because
// `nodeIntegration` is turned off. Use `preload.js` to
// selectively enable features needed in the rendering
// process.

document.getElementById("invoke").addEventListener("click", () => {
    var sdk = new Module.SDKWebAssembly()
    sdk.invokeAlgorithmFunc(5, (code, message) => {
        document.getElementById("title").innerText = code + ": " + message
    })
    sdk.delete()
})