const { Router } = require('express')
const CategoriaController = require('../controllers/CategoriaController')

const router = Router()

router.get('/categorias', CategoriaController.pegaTodosOscategorias)
router.get('/categorias/:id', CategoriaController.pegaUmaCategoria)
router.post('/categorias', CategoriaController.criaCategoria)
router.put('/categorias/:id', CategoriaController.atualizarCategoria)
router.delete('/categorias/:id', CategoriaController.apagaCategoria)
router.post('/categorias/:id/restaura', CategoriaController.restauraCategoria)

module.exports = router
